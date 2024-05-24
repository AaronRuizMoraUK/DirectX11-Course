#include <RHI/Resource/Views/RenderTargetView.h>

#include <RHI/Device/Device.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <RHI/DirectX/Utils.h>

namespace DX
{
    static D3D11_RTV_DIMENSION ToDX11RenderTargetViewDimension(const Texture& texture)
    {
        D3D11_RTV_DIMENSION rtvDimension;

        const TextureDesc& desc = texture.GetTextureDesc();

        switch (desc.m_textureType)
        {
        case TextureType::Unknown:
            rtvDimension = D3D11_RTV_DIMENSION_UNKNOWN;
            break;

        case TextureType::Texture1D:
            rtvDimension = (desc.m_arrayCount > 1) ? D3D11_RTV_DIMENSION_TEXTURE1DARRAY : D3D11_RTV_DIMENSION_TEXTURE1D;
            break;

        case TextureType::Texture2D:
            if (desc.m_sampleCount > 1)
            {
                rtvDimension = (desc.m_arrayCount > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY : D3D11_RTV_DIMENSION_TEXTURE2DMS;
            }
            else
            {
                rtvDimension = (desc.m_arrayCount > 1) ? D3D11_RTV_DIMENSION_TEXTURE2DARRAY : D3D11_RTV_DIMENSION_TEXTURE2D;
            }
            break;

        case TextureType::Texture3D:
            rtvDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
            break;

        default:
            DX_LOG(Error, "RenderTargetView", "Unknown texture type %d", desc.m_textureType);
            rtvDimension = D3D11_RTV_DIMENSION_UNKNOWN;
        }

        return rtvDimension;
    }

    static D3D11_RENDER_TARGET_VIEW_DESC ToDX11RenderTargetViewDesc(const Texture& texture, const RenderTargetViewDesc& desc)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);
        rtvDesc.ViewDimension = ToDX11RenderTargetViewDimension(texture);

        // View boundaries to the texture
        switch (rtvDesc.ViewDimension)
        {
        case D3D11_RTV_DIMENSION_TEXTURE1D:
            rtvDesc.Texture1D.MipSlice = desc.m_firstMip;
            break;

        case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:
            rtvDesc.Texture1DArray.MipSlice = desc.m_firstMip;
            rtvDesc.Texture1DArray.FirstArraySlice = desc.m_firstArray;
            rtvDesc.Texture1DArray.ArraySize = desc.m_arrayCount;
            break;

        case D3D11_RTV_DIMENSION_TEXTURE2D:
            rtvDesc.Texture2D.MipSlice = desc.m_firstMip;
            break;

        case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:
            rtvDesc.Texture2DArray.MipSlice = desc.m_firstMip;
            rtvDesc.Texture2DArray.FirstArraySlice = desc.m_firstArray;
            rtvDesc.Texture2DArray.ArraySize = desc.m_arrayCount;
            break;

        case D3D11_RTV_DIMENSION_TEXTURE2DMS:
            // Nothing to be set for this view dimension.
            break;

        case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY:
            rtvDesc.Texture2DMSArray.FirstArraySlice = desc.m_firstArray;
            rtvDesc.Texture2DMSArray.ArraySize = desc.m_arrayCount;
            break;

        case D3D11_RTV_DIMENSION_TEXTURE3D:
            rtvDesc.Texture3D.MipSlice = desc.m_firstMip;
            rtvDesc.Texture3D.FirstWSlice = desc.m_firstDepth;
            rtvDesc.Texture3D.WSize = desc.m_depthCount;
            break;

        case D3D11_RTV_DIMENSION_UNKNOWN:
        default:
            DX_LOG(Error, "RenderTargetView", "Render target view dimension unknown.");
            break;
        }

        return rtvDesc;
    }

    static D3D11_RENDER_TARGET_VIEW_DESC ToDX11RenderTargetViewDesc(const Buffer& buffer, const RenderTargetViewDesc& desc)
    {
        if (buffer.GetBufferDesc().m_bufferSubType != BufferSubType::Typed)
        {
            DX_LOG(Error, "RenderTargetView", "Only Typed Buffer is supported in Render Target View.");
        }

        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);
        rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_BUFFER;

        // View boundaries to the buffer
        rtvDesc.Buffer.FirstElement = desc.m_firstElement;
        rtvDesc.Buffer.NumElements = desc.m_elementCount;

        return rtvDesc;
    }

    RenderTargetView::RenderTargetView(Device* device, const RenderTargetViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // View of a texture
        if (auto* texture = std::get_if<std::shared_ptr<Texture>>(&desc.m_resource))
        {
            if (texture->get() == nullptr)
            {
                DX_LOG(Fatal, "RenderTargetView", "Render Target View description with invalid texture resource.");
                return;
            }

            if (!(texture->get()->GetTextureDesc().m_bindFlags & TextureBind_RenderTarget))
            {
                DX_LOG(Fatal, "RenderTargetView", "Render Target View description with texture that doesn't have the render target flag.");
                return;
            }

            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = ToDX11RenderTargetViewDesc(*texture->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateRenderTargetView(
                texture->get()->GetDX11Texture().Get(),
                &rtvDesc,
                m_dx11RenderTargetView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "RenderTargetView", "Failed to create Render Target View with a Texture.");
                return;
            }
        }
        // View of a buffer
        else if (auto* buffer = std::get_if<std::shared_ptr<Buffer>>(&desc.m_resource))
        {
            if (buffer->get() == nullptr)
            {
                DX_LOG(Fatal, "RenderTargetView", "Render Target View description with invalid buffer resource.");
                return;
            }

            if (!(buffer->get()->GetBufferDesc().m_bindFlags & BufferBind_RenderTarget))
            {
                DX_LOG(Fatal, "RenderTargetView", "Render Target View description with buffer that doesn't have the render target flag.");
                return;
            }

            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = ToDX11RenderTargetViewDesc(*buffer->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateRenderTargetView(
                buffer->get()->GetDX11Buffer().Get(),
                &rtvDesc,
                m_dx11RenderTargetView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "RenderTargetView", "Failed to create Render Target View with a Buffer.");
                return;
            }
        }
        else
        {
            DX_LOG(Fatal, "RenderTargetView", "Render Target View description with invalid resource.");
            return;
        }

        //DX_LOG(Verbose, "RenderTargetView", "Render Target View created.");
    }

    RenderTargetView::~RenderTargetView()
    {
        //DX_LOG(Verbose, "RenderTargetView", "Render Target View destroyed.");
    }

    ComPtr<ID3D11RenderTargetView> RenderTargetView::GetDX11RenderTargetView()
    {
        return m_dx11RenderTargetView;
    }
} // namespace DX
