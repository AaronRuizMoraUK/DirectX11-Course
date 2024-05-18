#include <Graphics/Resource/Views/ShaderResourceView.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    static D3D11_SRV_DIMENSION ToDX11ShaderResourceViewDimension(const Texture& texture)
    {
        D3D11_SRV_DIMENSION srvDimension;

        const TextureDesc& desc = texture.GetTextureDesc();

        switch (desc.m_textureType)
        {
        case TextureType::Unknown:
            srvDimension = D3D11_SRV_DIMENSION_UNKNOWN;
            break;
        
        case TextureType::Texture1D:
            srvDimension = (desc.m_arrayCount > 1) ? D3D11_SRV_DIMENSION_TEXTURE1DARRAY : D3D11_SRV_DIMENSION_TEXTURE1D;
            break;
        
        case TextureType::Texture2D:
            if (desc.m_sampleCount > 1)
            {
                srvDimension = (desc.m_arrayCount > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY : D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
            else
            {
                srvDimension = (desc.m_arrayCount > 1) ? D3D11_SRV_DIMENSION_TEXTURE2DARRAY : D3D11_SRV_DIMENSION_TEXTURE2D;
            }
            break;

        case TextureType::TextureCube:
            DX_ASSERT(desc.m_arrayCount % 6 == 0, "ShaderResourceView", "Texture array size for TextureCube must be a multiple of 6, but passed %d" , desc.m_arrayCount);
            srvDimension = (desc.m_arrayCount > 6) ? D3D11_SRV_DIMENSION_TEXTURECUBEARRAY : D3D11_SRV_DIMENSION_TEXTURECUBE;
            break;
        
        case TextureType::Texture3D:
            srvDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
            break;
        
        default:
            DX_LOG(Error, "ShaderResourceView", "Unknown texture type %d", desc.m_textureType);
            srvDimension = D3D11_SRV_DIMENSION_UNKNOWN;
        }

        return srvDimension;
    }

    static D3D11_SHADER_RESOURCE_VIEW_DESC ToDX11ShaderResourceViewDesc(const Texture& texture, const ShaderResourceViewDesc& desc)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);
        srvDesc.ViewDimension = ToDX11ShaderResourceViewDimension(texture);

        // View boundaries to the texture
        switch (srvDesc.ViewDimension)
        {
        case D3D11_SRV_DIMENSION_TEXTURE1D:
            srvDesc.Texture1D.MostDetailedMip = desc.m_firstMip;
            srvDesc.Texture1D.MipLevels = desc.m_mipCount;
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
            srvDesc.Texture1DArray.MostDetailedMip = desc.m_firstMip;
            srvDesc.Texture1DArray.MipLevels = desc.m_mipCount;
            srvDesc.Texture1DArray.FirstArraySlice = desc.m_firstArray;
            srvDesc.Texture1DArray.ArraySize = desc.m_arrayCount;
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE2D:
            srvDesc.Texture2D.MostDetailedMip = desc.m_firstMip;
            srvDesc.Texture2D.MipLevels = desc.m_mipCount;
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
            srvDesc.Texture2DArray.MostDetailedMip = desc.m_firstMip;
            srvDesc.Texture2DArray.MipLevels = desc.m_mipCount;
            srvDesc.Texture2DArray.FirstArraySlice = desc.m_firstArray;
            srvDesc.Texture2DArray.ArraySize = desc.m_arrayCount;
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE2DMS:
            // Nothing to be set for this view dimension.
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
            srvDesc.Texture2DMSArray.FirstArraySlice = desc.m_firstArray;
            srvDesc.Texture2DMSArray.ArraySize = desc.m_arrayCount;
            break;

        case D3D11_SRV_DIMENSION_TEXTURECUBE:
            srvDesc.TextureCube.MostDetailedMip = desc.m_firstMip;
            srvDesc.TextureCube.MipLevels = desc.m_mipCount;
            break;

        case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
            srvDesc.TextureCubeArray.MostDetailedMip = desc.m_firstMip;
            srvDesc.TextureCubeArray.MipLevels = desc.m_mipCount;
            srvDesc.TextureCubeArray.First2DArrayFace = desc.m_firstArray;
            srvDesc.TextureCubeArray.NumCubes = desc.m_arrayCount;
            break;
        
        case D3D11_SRV_DIMENSION_TEXTURE3D:
            srvDesc.Texture3D.MostDetailedMip = desc.m_firstMip;
            srvDesc.Texture3D.MipLevels = desc.m_mipCount;
            break;
        
        case D3D11_SRV_DIMENSION_UNKNOWN:
        default:
            DX_LOG(Error, "ShaderResourceView", "Shader resource view dimension unknown.");
            break;
        }

        return srvDesc;
    }

    static D3D11_SHADER_RESOURCE_VIEW_DESC ToDX11ShaderResourceViewDesc(const Buffer& buffer, const ShaderResourceViewDesc& desc)
    {
        if (buffer.GetBufferDesc().m_bufferType == BufferType::None)
        {
            DX_LOG(Error, "ShaderResourceView", "Unexpected Buffer type None in Shader Resource View.");
        }
        else if (buffer.GetBufferDesc().m_bufferType == BufferType::Structured &&
            desc.m_viewFormat != ResourceFormat::Unknown)
        {
            DX_LOG(Error, "ShaderResourceView", "Structured buffer only supports Unknown view format in Shader Resource View.");
        }
        else if (buffer.GetBufferDesc().m_bufferType == BufferType::Raw &&
            desc.m_viewFormat != ResourceFormat::R32_TYPELESS)
        {
            DX_LOG(Error, "ShaderResourceView", "Raw buffer only supports R32_TYPELESS view format in Shader Resource View.");
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);

        switch (buffer.GetBufferDesc().m_bufferType)
        {
        case BufferType::None:
            break;

        case BufferType::Typed:
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

            // View boundaries to the buffer
            srvDesc.Buffer.FirstElement = desc.m_firstElement;
            srvDesc.Buffer.NumElements = desc.m_elementCount;
            break;

        case BufferType::Structured:
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;

            // View boundaries to the buffer
            srvDesc.BufferEx.FirstElement = desc.m_firstElement;
            srvDesc.BufferEx.NumElements = desc.m_elementCount;
            srvDesc.BufferEx.Flags = 0;
            break;

        case BufferType::Raw:
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;

            // View boundaries to the buffer
            srvDesc.BufferEx.FirstElement = desc.m_firstElement;
            srvDesc.BufferEx.NumElements = desc.m_elementCount;
            srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
            break;

        default:
            DX_LOG(Error, "ShaderResourceView", "Unknown buffer type %d", buffer.GetBufferDesc().m_bufferType);
            break;
        }

        return srvDesc;
    }

    ShaderResourceView::ShaderResourceView(Device* device, const ShaderResourceViewDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // View of a texture
        if (auto* texture = std::get_if<std::shared_ptr<Texture>>(&desc.m_resource))
        {
            if (texture->get() == nullptr)
            {
                DX_LOG(Fatal, "ShaderResourceView", "Shader Resource View description with invalid texture resource.");
                return;
            }

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = ToDX11ShaderResourceViewDesc(*texture->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateShaderResourceView(
                texture->get()->GetDX11Texture().Get(),
                &srvDesc,
                m_dx11ShaderResourceView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "ShaderResourceView", "Failed to create Shader Resource View with a Texture.");
                return;
            }
        }
        // View of a buffer
        else if (auto* buffer = std::get_if<std::shared_ptr<Buffer>>(&desc.m_resource))
        {
            if (buffer->get() == nullptr)
            {
                DX_LOG(Fatal, "ShaderResourceView", "Shader Resource View description with invalid buffer resource.");
                return;
            }

            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = ToDX11ShaderResourceViewDesc(*buffer->get(), desc);

            auto result = m_ownerDevice->GetDX11Device()->CreateShaderResourceView(
                buffer->get()->GetDX11Buffer().Get(),
                &srvDesc,
                m_dx11ShaderResourceView.GetAddressOf()
            );

            if (FAILED(result))
            {
                DX_LOG(Fatal, "ShaderResourceView", "Failed to create Shader Resource View with a Buffer.");
                return;
            }
        }
        else
        {
            DX_LOG(Fatal, "ShaderResourceView", "Shader Resource View description with invalid resource.");
            return;
        }

        DX_LOG(Verbose, "ShaderResourceView", "Shader Resource View created.");
    }

    ShaderResourceView::~ShaderResourceView()
    {
        DX_LOG(Verbose, "ShaderResourceView", "Shader Resource View destroyed.");
    }

    ComPtr<ID3D11ShaderResourceView> ShaderResourceView::GetDXShaderResourceView()
    {
        return m_dx11ShaderResourceView;
    }
} // namespace DX
