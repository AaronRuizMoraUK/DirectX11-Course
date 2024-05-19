#include <Graphics/FrameBuffer/FrameBuffer.h>

#include <Graphics/Device/Device.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Resource/Views/RenderTargetView.h>
#include <Graphics/Resource/Views/DepthStencilView.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    FrameBuffer::FrameBuffer(Device* device, const FrameBufferDesc& desc)
        : DeviceObject(device)
    {
        m_colorTexture = desc.m_colorAttachment;

        // Create a default depth stencil texture
        if (m_colorTexture && desc.m_createDepthStencilAttachment)
        {
            const TextureDesc& colorTextureDesc = m_colorTexture->GetTextureDesc();

            TextureDesc depthStencilTextureDesc = {};
            depthStencilTextureDesc.m_textureType = TextureType::Texture2D;
            depthStencilTextureDesc.m_dimensions = colorTextureDesc.m_dimensions;
            depthStencilTextureDesc.m_mipCount = colorTextureDesc.m_mipCount;
            depthStencilTextureDesc.m_format = ResourceFormat::D24_UNORM_S8_UINT;
            depthStencilTextureDesc.m_usage = ResourceUsage::Default;
            depthStencilTextureDesc.m_bindFlags = TextureBind_DepthStencil;
            depthStencilTextureDesc.m_cpuAccess = ResourceCPUAccess::None;
            depthStencilTextureDesc.m_arrayCount = colorTextureDesc.m_arrayCount;
            depthStencilTextureDesc.m_sampleCount = colorTextureDesc.m_sampleCount;
            depthStencilTextureDesc.m_sampleQuality = colorTextureDesc.m_sampleQuality;

            m_depthStencilTexture = m_ownerDevice->CreateTexture(depthStencilTextureDesc);
        }
        else
        {
            m_depthStencilTexture = desc.m_depthStencilAttachment;
        }

        if (m_colorTexture)
        {
            RenderTargetViewDesc rtvDesc = {};
            rtvDesc.m_resource = m_colorTexture;
            rtvDesc.m_viewFormat = m_colorTexture->GetTextureDesc().m_format;
            rtvDesc.m_firstMip =0;
            rtvDesc.m_firstArray = 0;
            rtvDesc.m_arrayCount = m_colorTexture->GetTextureDesc().m_arrayCount;
            rtvDesc.m_firstDepth = 0;
            rtvDesc.m_depthCount = -1;

            m_colorRenderTargetView = m_ownerDevice->CreateRenderTargetView(rtvDesc);
        }

        if (m_depthStencilTexture)
        {
            DepthStencilViewDesc dsvDesc = {};
            dsvDesc.m_texture = m_depthStencilTexture;
            dsvDesc.m_viewFormat = m_depthStencilTexture->GetTextureDesc().m_format;
            dsvDesc.m_firstMip = 0;
            dsvDesc.m_firstArray = 0;
            dsvDesc.m_arrayCount = m_depthStencilTexture->GetTextureDesc().m_arrayCount;

            m_depthStencilView = m_ownerDevice->CreateDepthStencilView(dsvDesc);
        }

        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer created. Color: %s DepthStencil: %s", 
            (m_colorRenderTargetView) ? "YES" : "NO",
            (m_depthStencilView) ? "YES" : "NO");
    }

    FrameBuffer::~FrameBuffer()
    {
        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer destroyed.");
    }

    void FrameBuffer::Clear(std::optional<mathfu::Color> color, std::optional<float> depth, std::optional<uint8_t> stencil)
    {
        if (m_colorRenderTargetView && color.has_value())
        {
            m_ownerDevice->GetDX11ImmediateContext()->ClearRenderTargetView(
                m_colorRenderTargetView->GetDX11RenderTargetView().Get(), mathfu::ColorPacked(*color).data_);
        }

        if (m_depthStencilView)
        {
            if (depth.has_value() || stencil.has_value())
            {
                uint32_t flags = (depth.has_value() ? D3D11_CLEAR_DEPTH : 0) | (stencil.has_value() ? D3D11_CLEAR_STENCIL : 0);

                m_ownerDevice->GetDX11ImmediateContext()->ClearDepthStencilView(
                    m_depthStencilView->GetDX11DepthStencilView().Get(), flags, depth.value_or(1.0f), stencil.value_or(0));
            }
        }
    }

    std::shared_ptr<RenderTargetView> FrameBuffer::GetColorRenderTargetView()
    {
        return m_colorRenderTargetView;
    }

    std::shared_ptr<DepthStencilView> FrameBuffer::GetDepthStencilView()
    {
        return m_depthStencilView;
    }
} // namespace DX
