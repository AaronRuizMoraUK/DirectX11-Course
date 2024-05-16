#include <Graphics/FrameBuffer/FrameBuffer.h>

#include <Graphics/Device/Device.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

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
            depthStencilTextureDesc.m_variant = TextureVariant::Texture2D;
            depthStencilTextureDesc.m_size = colorTextureDesc.m_size;
            depthStencilTextureDesc.m_mipLevels = colorTextureDesc.m_mipLevels;
            depthStencilTextureDesc.m_format = ResourceFormat::D24_UNORM_S8_UINT;
            depthStencilTextureDesc.m_usage = ResourceUsage::Default;
            depthStencilTextureDesc.m_bindFlag = ResourceBind_DepthStencil;
            depthStencilTextureDesc.m_cpuAccess = ResourceCPUAccess::None;
            depthStencilTextureDesc.m_arraySize = colorTextureDesc.m_arraySize;
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
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = ToDX11ResourceFormat(m_colorTexture->GetTextureDesc().m_format);
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // TODO: Create function to obtain RTV dimension from the texture.
            rtvDesc.Texture2D.MipSlice = 0; // Index of first mipmap level to use.

            auto result = m_ownerDevice->GetDX11Device()->CreateRenderTargetView(
                m_colorTexture->GetDX11Texture().Get(),
                &rtvDesc,
                m_dx11ColorRenderTargetView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "FrameBuffer", "Failed to create Color Render Target View.");
                return;
            }
        }

        if (m_depthStencilTexture)
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = ToDX11ResourceFormat(m_depthStencilTexture->GetTextureDesc().m_format);
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // TODO: Create function to obtain DSV dimension from the texture.
            dsvDesc.Texture2D.MipSlice = 0; // Index of first mipmap level to use.

            auto result = m_ownerDevice->GetDX11Device()->CreateDepthStencilView(
                m_depthStencilTexture->GetDX11Texture().Get(),
                &dsvDesc,
                m_dx11DepthStencilView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "FrameBuffer", "Failed to create Depth Stencil View.");
                return;
            }
        }

        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer created. Color: %s DepthStencil: %s", 
            (m_dx11ColorRenderTargetView) ? "YES" : "NO",
            (m_dx11DepthStencilView) ? "YES" : "NO");
    }

    FrameBuffer::~FrameBuffer()
    {
        DX_LOG(Info, "FrameBuffer", "Graphics frame buffer destroyed.");
    }

    void FrameBuffer::Clear(std::optional<mathfu::Color> color, std::optional<float> depth, std::optional<uint8_t> stencil)
    {
        if (color.has_value())
        {
            m_ownerDevice->GetDX11ImmediateContext()->ClearRenderTargetView(
                m_dx11ColorRenderTargetView.Get(), mathfu::ColorPacked(*color).data_);
        }

        if (depth.has_value() || stencil.has_value())
        {
            uint32_t flags = (depth.has_value() ? D3D11_CLEAR_DEPTH : 0) | (stencil.has_value() ? D3D11_CLEAR_STENCIL : 0);

            m_ownerDevice->GetDX11ImmediateContext()->ClearDepthStencilView(
                m_dx11DepthStencilView.Get(), flags, depth.value_or(0.0f), stencil.value_or(0));
        }
    }
} // namespace DX
