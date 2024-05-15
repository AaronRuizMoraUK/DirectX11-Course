#include <Graphics/FrameBuffer/FrameBuffer.h>

#include <Graphics/Device/Device.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/TextureUtils.h>

namespace DX
{
    FrameBuffer::FrameBuffer(Device* device, const FrameBufferDesc& desc)
        : DeviceObject(device)
    {
        // Create a frame buffer from a SwapChain
        if (desc.m_swapChain)
        {
            m_dx11ColorTexture = desc.m_swapChain->GetBackBuffer();

            // Create a default depth stencil texture
            {
                D3D11_TEXTURE2D_DESC dx11ColorTextureDesc = {};
                m_dx11ColorTexture->GetDesc(&dx11ColorTextureDesc);

                D3D11_TEXTURE2D_DESC dx11DepthStencialTextureDesc = {};
                dx11DepthStencialTextureDesc.Width = dx11ColorTextureDesc.Width;
                dx11DepthStencialTextureDesc.Height = dx11ColorTextureDesc.Height;
                dx11DepthStencialTextureDesc.MipLevels = 1;
                dx11DepthStencialTextureDesc.ArraySize = 1;
                dx11DepthStencialTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
                dx11DepthStencialTextureDesc.SampleDesc.Count = 1;
                dx11DepthStencialTextureDesc.SampleDesc.Quality = 0;
                dx11DepthStencialTextureDesc.Usage = D3D11_USAGE_DEFAULT;
                dx11DepthStencialTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
                dx11DepthStencialTextureDesc.CPUAccessFlags = 0;
                dx11DepthStencialTextureDesc.MiscFlags = 0;

                auto result = m_ownerDevice->GetDX11Device()->CreateTexture2D(
                    &dx11DepthStencialTextureDesc, 
                    nullptr, // No sub-resource data
                    m_dx11DepthStencilTexture.GetAddressOf());

                if (FAILED(result))
                {
                    DX_LOG(Fatal, "FrameBuffer", "Failed to create Depth Stencil texture.");
                    return;
                }
            }
        }
        // Create a frame buffer from textures passed in the description
        else
        {
            // TODO: To be implemented with Texture class is done.
        }

        if (m_dx11ColorTexture)
        {
            D3D11_TEXTURE2D_DESC dx11ColorTextureDesc = {};
            m_dx11ColorTexture->GetDesc(&dx11ColorTextureDesc);

            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = dx11ColorTextureDesc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;

            auto result = m_ownerDevice->GetDX11Device()->CreateRenderTargetView(
                m_dx11ColorTexture.Get(),
                &rtvDesc,
                m_dx11ColorRenderTargetView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "FrameBuffer", "Failed to create Color Render Target View.");
                return;
            }
        }

        if (m_dx11DepthStencilTexture)
        {
            D3D11_TEXTURE2D_DESC dx11DepthStencilTextureDesc = {};
            m_dx11DepthStencilTexture->GetDesc(&dx11DepthStencilTextureDesc);

            D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
            dsvDesc.Format = dx11DepthStencilTextureDesc.Format;
            dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            dsvDesc.Texture2D.MipSlice = 0;

            auto result = m_ownerDevice->GetDX11Device()->CreateDepthStencilView(
                m_dx11DepthStencilTexture.Get(),
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
