#include <Graphics/Resource/Views/ShaderRWResourceView.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Resource/Buffer/Buffer.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    static D3D11_UAV_DIMENSION ToDX11UnorderedAccessViewDimension(const Texture& texture)
    {
        D3D11_UAV_DIMENSION uavDimension;

        const TextureDesc& desc = texture.GetTextureDesc();

        switch (desc.m_variant)
        {
        case TextureVariant::Unknown:
            uavDimension = D3D11_UAV_DIMENSION_UNKNOWN;
            break;

        case TextureVariant::Texture1D:
            uavDimension = (desc.m_arraySize > 0) ? D3D11_UAV_DIMENSION_TEXTURE1DARRAY : D3D11_UAV_DIMENSION_TEXTURE1D;
            break;

        case TextureVariant::Texture2D:
        case TextureVariant::TextureCube: // TODO: Verify it works. Only different with Texture2D is D3D11_RESOURCE_MISC_TEXTURECUBE misc flag.
            uavDimension = (desc.m_arraySize > 0) ? D3D11_UAV_DIMENSION_TEXTURE2DARRAY : D3D11_UAV_DIMENSION_TEXTURE2D;
            break;

        case TextureVariant::Texture3D:
            uavDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
            break;

        default:
            DX_LOG(Error, "ShaderRWResourceView", "Unknown texture variant %d", desc.m_variant);
            uavDimension = D3D11_UAV_DIMENSION_UNKNOWN;
        }

        return uavDimension;
    }

    static D3D11_UNORDERED_ACCESS_VIEW_DESC ToDX11UnorderedAccessViewDesc(const Texture& texture, const ShaderRWResourceViewDesc& desc)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = ToDX11ResourceFormat(texture.GetTextureDesc().m_format);
        uavDesc.ViewDimension = ToDX11UnorderedAccessViewDimension(texture);

        // View boundaries to the texture
        switch (uavDesc.ViewDimension)
        {
        case D3D11_UAV_DIMENSION_TEXTURE1D:
            uavDesc.Texture1D.MipSlice = desc.m_firstMip;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE1DARRAY:
            uavDesc.Texture1DArray.MipSlice = desc.m_firstMip;
            uavDesc.Texture1DArray.FirstArraySlice = desc.m_firstArray;
            uavDesc.Texture1DArray.ArraySize = desc.m_arraySize;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE2D:
            uavDesc.Texture2D.MipSlice = desc.m_firstMip;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE2DARRAY:
            uavDesc.Texture2DArray.MipSlice = desc.m_firstMip;
            uavDesc.Texture2DArray.FirstArraySlice = desc.m_firstArray;
            uavDesc.Texture2DArray.ArraySize = desc.m_arraySize;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE3D:
            uavDesc.Texture3D.MipSlice = desc.m_firstMip;
            uavDesc.Texture3D.FirstWSlice = desc.m_firstDepth;
            uavDesc.Texture3D.WSize = desc.m_depthSize;
            break;
        
        case D3D11_UAV_DIMENSION_UNKNOWN:
        default:
            DX_LOG(Error, "ShaderRWResourceView", "Shader RW resource view dimension unknown.");
            break;
        }

        return uavDesc;
    }

    static D3D11_UNORDERED_ACCESS_VIEW_DESC ToDX11UnorderedAccessViewDesc(const Buffer& buffer, const ShaderRWResourceViewDesc& desc)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = ToDX11ResourceFormat(buffer.GetBufferDesc().m_format);
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

        // View boundaries to the buffer
        uavDesc.Buffer.FirstElement = desc.m_firstElement;
        uavDesc.Buffer.NumElements = desc.m_numElements;
        uavDesc.Buffer.Flags = 0;

        return uavDesc;
    }

    ShaderRWResourceView::ShaderRWResourceView(Device* device, const ShaderRWResourceViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // View of a texture
        if (auto* texture = std::get_if<std::shared_ptr<Texture>>(&desc.m_resource))
        {
            if (texture->get() == nullptr)
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Shader RW Resource View description with invalid texture resource.");
                return;
            }

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = ToDX11UnorderedAccessViewDesc(*texture->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateUnorderedAccessView(
                texture->get()->GetDX11Texture().Get(),
                &uavDesc,
                m_dx11UnorderedAccessView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Failed to create Shader RW Resource View with a Texture.");
                return;
            }
        }
        // View of a buffer
        else if (auto* buffer = std::get_if<std::shared_ptr<Buffer>>(&desc.m_resource))
        {
            if (buffer->get() == nullptr)
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Shader RW Resource View description with invalid buffer resource.");
                return;
            }

            D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = ToDX11UnorderedAccessViewDesc(*buffer->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateUnorderedAccessView(
                buffer->get()->GetDX11Buffer().Get(),
                &uavDesc,
                m_dx11UnorderedAccessView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Failed to create Shader RW Resource View with a Buffer.");
                return;
            }
        }
        else
        {
            DX_LOG(Fatal, "ShaderRWResourceView", "Shader RW Resource View description with invalid resource.");
            return;
        }

        DX_LOG(Verbose, "ShaderRWResourceView", "Shader RW Resource View created.");
    }

    ShaderRWResourceView::~ShaderRWResourceView()
    {
        DX_LOG(Verbose, "ShaderRWResourceView", "Shader RW Resource View destroyed.");
    }
} // namespace DX
