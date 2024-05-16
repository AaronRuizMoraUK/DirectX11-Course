#include <Graphics/Resource/Views/DepthStencilView.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    static D3D11_DSV_DIMENSION ToDX11DepthStencilViewDimension(const Texture& texture)
    {
        D3D11_DSV_DIMENSION dsvDimension;

        const TextureDesc& desc = texture.GetTextureDesc();

        switch (desc.m_variant)
        {
        case TextureVariant::Unknown:
            dsvDimension = D3D11_DSV_DIMENSION_UNKNOWN;
            break;

        case TextureVariant::Texture1D:
            dsvDimension = (desc.m_arraySize > 0) ? D3D11_DSV_DIMENSION_TEXTURE1DARRAY : D3D11_DSV_DIMENSION_TEXTURE1D;
            break;

        case TextureVariant::Texture2D:
            if (desc.m_sampleCount > 1)
            {
                dsvDimension = (desc.m_arraySize > 0) ? D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY : D3D11_DSV_DIMENSION_TEXTURE2DMS;
            }
            else
            {
                dsvDimension = (desc.m_arraySize > 0) ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D;
            }
            break;

        case TextureVariant::Texture3D:
            DX_LOG(Error, "DepthStencilView", "Depth stencil view does not support 3D textures");
            dsvDimension = D3D11_DSV_DIMENSION_UNKNOWN;
            break;

        default:
            DX_LOG(Error, "DepthStencilView", "Unknown texture variant %d", desc.m_variant);
            dsvDimension = D3D11_DSV_DIMENSION_UNKNOWN;
            break;
        }

        return dsvDimension;
    }

    static D3D11_DEPTH_STENCIL_VIEW_DESC ToDX11DepthStencilViewDesc(const Texture& texture, const DepthStencilViewDesc& desc)
    {
        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
        dsvDesc.Format = ToDX11ResourceFormat(texture.GetTextureDesc().m_format);
        dsvDesc.ViewDimension = ToDX11DepthStencilViewDimension(texture);

        // View boundaries to the texture
        switch (dsvDesc.ViewDimension)
        {
        case D3D11_DSV_DIMENSION_TEXTURE1D:
            dsvDesc.Texture1D.MipSlice = desc.m_firstMip;
            break;

        case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:
            dsvDesc.Texture1DArray.MipSlice = desc.m_firstMip;
            dsvDesc.Texture1DArray.FirstArraySlice = desc.m_firstArray;
            dsvDesc.Texture1DArray.ArraySize = desc.m_arraySize;
            break;

        case D3D11_DSV_DIMENSION_TEXTURE2D:
            dsvDesc.Texture2D.MipSlice = desc.m_firstMip;
            break;

        case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:
            dsvDesc.Texture2DArray.MipSlice = desc.m_firstMip;
            dsvDesc.Texture2DArray.FirstArraySlice = desc.m_firstArray;
            dsvDesc.Texture2DArray.ArraySize = desc.m_arraySize;
            break;

        case D3D11_DSV_DIMENSION_TEXTURE2DMS:
            // Nothing to be set for this view dimension.
            break;

        case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY:
            dsvDesc.Texture2DMSArray.FirstArraySlice = desc.m_firstArray;
            dsvDesc.Texture2DMSArray.ArraySize = desc.m_arraySize;
            break;

        case D3D11_DSV_DIMENSION_UNKNOWN:
        default:
            DX_LOG(Error, "DepthStencilView", "Depth stencil view dimension unknown.");
            break;
        }

        return dsvDesc;
    }

    DepthStencilView::DepthStencilView(Device* device, const DepthStencilViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (!desc.m_texture)
        {
            DX_LOG(Fatal, "SwapChain", "Swap chain description with invalid texture.");
            return;
        }

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = ToDX11DepthStencilViewDesc(*desc.m_texture.get(), desc);

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
