#include <RHI/FrameBuffer/FrameBuffer.h>

#include <RHI/Device/Device.h>
#include <RHI/SwapChain/SwapChain.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Views/RenderTargetView.h>
#include <RHI/Resource/Views/DepthStencilView.h>
#include <Log/Log.h>
#include <Debug/Debug.h>

namespace DX
{
    FrameBuffer::FrameBuffer(Device* device, const FrameBufferDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // Create a default depth stencil texture using the dimensions of the first texture render target
        if (!m_desc.m_depthStencilAttachment.m_texture &&
            m_desc.m_createDepthStencilAttachment)
        {
            if (auto* textures = std::get_if<FrameBufferDesc::TextureAttachments>(&m_desc.m_renderTargetAttachments);
                textures && !textures->empty())
            {
                const TextureDesc& textureDesc = textures->front().m_texture->GetTextureDesc();

                TextureDesc depthStencilTextureDesc = {};
                depthStencilTextureDesc.m_textureType = TextureType::Texture2D;
                depthStencilTextureDesc.m_dimensions = textureDesc.m_dimensions;
                depthStencilTextureDesc.m_mipCount = textureDesc.m_mipCount;
                depthStencilTextureDesc.m_format = ResourceFormat::D24_UNORM_S8_UINT;
                depthStencilTextureDesc.m_usage = ResourceUsage::Default;
                depthStencilTextureDesc.m_bindFlags = TextureBind_DepthStencil;
                depthStencilTextureDesc.m_cpuAccess = ResourceCPUAccess::None;
                depthStencilTextureDesc.m_arrayCount = textureDesc.m_arrayCount;
                depthStencilTextureDesc.m_sampleCount = textureDesc.m_sampleCount;
                depthStencilTextureDesc.m_sampleQuality = textureDesc.m_sampleQuality;
                depthStencilTextureDesc.m_initialData = nullptr;

                // NOTE: Not created through owner device API to avoid having a
                // reference in the device as this is a sub-object of FrameBuffer.
                m_desc.m_depthStencilAttachment.m_texture = std::make_shared<Texture>(m_ownerDevice, depthStencilTextureDesc);
                m_desc.m_depthStencilAttachment.m_viewFormat = depthStencilTextureDesc.m_format;
            }
        }

        if (auto* textures = std::get_if<FrameBufferDesc::TextureAttachments>(&m_desc.m_renderTargetAttachments);
            textures && !textures->empty())
        {
            m_renderTargetViews.resize(textures->size());
            std::transform(textures->begin(), textures->end(), m_renderTargetViews.begin(),
                [this](const FrameBufferDesc::TextureAttachment& textureAtt)
                {
                    RenderTargetViewDesc rtvDesc = {};
                    rtvDesc.m_resource = textureAtt.m_texture;
                    rtvDesc.m_viewFormat = textureAtt.m_viewFormat;
                    rtvDesc.m_firstMip = 0;
                    rtvDesc.m_firstArray = 0;
                    rtvDesc.m_arrayCount = textureAtt.m_texture->GetTextureDesc().m_arrayCount;
                    rtvDesc.m_firstDepth = 0;
                    rtvDesc.m_depthCount = textureAtt.m_texture->GetTextureDesc().m_dimensions.z;

                    // NOTE: Not created through owner device API to avoid having a
                    // reference in the device as this is a sub-object of FrameBuffer.
                    return std::make_shared<RenderTargetView>(m_ownerDevice, rtvDesc);
                });
        }
        else if (auto* buffers = std::get_if<FrameBufferDesc::BufferAttachments>(&m_desc.m_renderTargetAttachments);
            buffers && !buffers->empty())
        {
            m_renderTargetViews.resize(buffers->size());
            std::transform(buffers->begin(), buffers->end(), m_renderTargetViews.begin(),
                [this](const FrameBufferDesc::BufferAttachment& bufferAtt)
                {
                    RenderTargetViewDesc rtvDesc = {};
                    rtvDesc.m_resource = bufferAtt.m_buffer;
                    rtvDesc.m_viewFormat = bufferAtt.m_viewFormat;
                    rtvDesc.m_firstElement = 0;
                    rtvDesc.m_elementCount = bufferAtt.m_buffer->GetBufferDesc().m_elementCount;

                    // NOTE: Not created through owner device API to avoid having a
                    // reference in the device as this is a sub-object of FrameBuffer.
                    return std::make_shared<RenderTargetView>(m_ownerDevice, rtvDesc);
                });
        }

        if (m_desc.m_depthStencilAttachment.m_texture)
        {
            DepthStencilViewDesc dsvDesc = {};
            dsvDesc.m_texture = m_desc.m_depthStencilAttachment.m_texture;
            dsvDesc.m_viewFormat = m_desc.m_depthStencilAttachment.m_viewFormat;
            dsvDesc.m_firstMip = 0;
            dsvDesc.m_firstArray = 0;
            dsvDesc.m_arrayCount = m_desc.m_depthStencilAttachment.m_texture->GetTextureDesc().m_arrayCount;

            // NOTE: Not created through owner device API to avoid having a
            // reference in the device as this is a sub-object of FrameBuffer.
            m_depthStencilView = std::make_shared<DepthStencilView>(m_ownerDevice, dsvDesc);
        }

        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer created. RenderTargets: %s (%d) DepthStencil: %s",
            (!m_renderTargetViews.empty()) ? "YES" : "NO", m_renderTargetViews.size(),
            (m_depthStencilView) ? "YES" : "NO");
    }

    FrameBuffer::~FrameBuffer()
    {
        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer destroyed.");
    }

    const std::vector<std::shared_ptr<RenderTargetView>>& FrameBuffer::GetRenderTargetViews()
    {
        return m_renderTargetViews;
    }

    std::shared_ptr<DepthStencilView> FrameBuffer::GetDepthStencilView()
    {
        return m_depthStencilView;
    }

    void FrameBuffer::FlipSwapChainBackBuffer(std::shared_ptr<Texture> backBuffer)
    {
        DX_ASSERT(m_renderTargetViews.size() == 1, "FrameBuffer",
            "Unexpected number of render target views. Expected 1, got %d", m_renderTargetViews.size());

        if (auto* textures = std::get_if<FrameBufferDesc::TextureAttachments>(&m_desc.m_renderTargetAttachments);
            textures && !textures->empty())
        {
            textures->front().m_texture = backBuffer;

            RenderTargetViewDesc rtvDesc = {};
            rtvDesc.m_resource = backBuffer;
            rtvDesc.m_viewFormat = textures->front().m_viewFormat;
            rtvDesc.m_firstMip = 0;
            rtvDesc.m_firstArray = 0;
            rtvDesc.m_arrayCount = backBuffer->GetTextureDesc().m_arrayCount;
            rtvDesc.m_firstDepth = 0;
            rtvDesc.m_depthCount = backBuffer->GetTextureDesc().m_dimensions.z;

            // NOTE: Not created through owner device API to avoid having a
            // reference in the device as this is a sub-object of FrameBuffer.
            m_renderTargetViews[0] = std::make_shared<RenderTargetView>(m_ownerDevice, rtvDesc);
        }
        else
        {
            DX_LOG(Fatal, "FrameBuffer", "Render target with no texture attachment. Cannot flip swap chain back buffer.");
        }
    }
} // namespace DX
