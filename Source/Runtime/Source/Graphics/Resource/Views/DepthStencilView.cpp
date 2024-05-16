#include <Graphics/Resource/Views/DepthStencilView.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    DepthStencilView::DepthStencilView(Device* device, const DepthStencilViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // TODO

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = ToDX11ResourceFormat(desc.m_texture->GetTextureDesc().m_format);
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // TODO: Create function to obtain DSV dimension from the texture.
        dsvDesc.Texture2D.MipSlice = 0; // Index of first mipmap level to use.

        auto result = m_ownerDevice->GetDX11Device()->CreateDepthStencilView(
            desc.m_texture->GetDX11Texture().Get(),
            &dsvDesc,
            m_dx11DepthStencilView.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Fatal, "DepthStencilView", "Failed to create Depth Stencil View.");
            return;
        }

        DX_LOG(Verbose, "DepthStencilView", "Depth Stencil View created.");
    }

    DepthStencilView::~DepthStencilView()
    {
        DX_LOG(Verbose, "DepthStencilView", "Depth Stencil View destroyed.");
    }

    void DepthStencilView::Clear(std::optional<float> depth, std::optional<uint8_t> stencil)
    {
        if (depth.has_value() || stencil.has_value())
        {
            uint32_t flags = (depth.has_value() ? D3D11_CLEAR_DEPTH : 0) | (stencil.has_value() ? D3D11_CLEAR_STENCIL : 0);

            m_ownerDevice->GetDX11ImmediateContext()->ClearDepthStencilView(
                m_dx11DepthStencilView.Get(), flags, depth.value_or(0.0f), stencil.value_or(0));
        }
    }
} // namespace DX
