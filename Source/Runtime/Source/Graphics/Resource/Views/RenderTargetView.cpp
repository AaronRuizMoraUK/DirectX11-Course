#include <Graphics/Resource/Views/RenderTargetView.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    RenderTargetView::RenderTargetView(Device* device, const RenderTargetViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // TODO

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = ToDX11ResourceFormat(std::get<std::shared_ptr<Texture>>(desc.m_resource)->GetTextureDesc().m_format);
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; // TODO: Create function to obtain RTV dimension from the texture.
        rtvDesc.Texture2D.MipSlice = 0; // Index of first mipmap level to use.

        auto result = m_ownerDevice->GetDX11Device()->CreateRenderTargetView(
            std::get<std::shared_ptr<Texture>>(desc.m_resource)->GetDX11Texture().Get(),
            &rtvDesc,
            m_dx11RenderTargetView.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Fatal, "RenderTargetView", "Failed to create Render Target View.");
            return;
        }

        DX_LOG(Verbose, "RenderTargetView", "Render Target View created.");
    }

    RenderTargetView::~RenderTargetView()
    {
        DX_LOG(Verbose, "RenderTargetView", "Render Target View destroyed.");
    }

    void RenderTargetView::Clear(const mathfu::Color& color)
    {
        m_ownerDevice->GetDX11ImmediateContext()->ClearRenderTargetView(
            m_dx11RenderTargetView.Get(), mathfu::ColorPacked(color).data_);
    }
} // namespace DX
