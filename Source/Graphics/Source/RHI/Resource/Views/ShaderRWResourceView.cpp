#include <RHI/Resource/Views/ShaderRWResourceView.h>

#include <RHI/Device/Device.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <RHI/DirectX/Utils.h>

namespace DX
{
    static D3D11_UAV_DIMENSION ToDX11UnorderedAccessViewDimension(const Texture& texture)
    {
        D3D11_UAV_DIMENSION uavDimension;

        const TextureDesc& desc = texture.GetTextureDesc();

        switch (desc.m_textureType)
        {
        case TextureType::Unknown:
            uavDimension = D3D11_UAV_DIMENSION_UNKNOWN;
            break;

        case TextureType::Texture1D:
            uavDimension = (desc.m_arrayCount > 1) ? D3D11_UAV_DIMENSION_TEXTURE1DARRAY : D3D11_UAV_DIMENSION_TEXTURE1D;
            break;

        case TextureType::Texture2D:
            uavDimension = (desc.m_arrayCount > 1) ? D3D11_UAV_DIMENSION_TEXTURE2DARRAY : D3D11_UAV_DIMENSION_TEXTURE2D;
            break;

        case TextureType::Texture3D:
            uavDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
            break;

        default:
            DX_LOG(Error, "ShaderRWResourceView", "Unknown texture type %d", desc.m_textureType);
            uavDimension = D3D11_UAV_DIMENSION_UNKNOWN;
        }

        return uavDimension;
    }

    static D3D11_UNORDERED_ACCESS_VIEW_DESC ToDX11UnorderedAccessViewDesc(const Texture& texture, const ShaderRWResourceViewDesc& desc)
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);
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
            uavDesc.Texture1DArray.ArraySize = desc.m_arrayCount;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE2D:
            uavDesc.Texture2D.MipSlice = desc.m_firstMip;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE2DARRAY:
            uavDesc.Texture2DArray.MipSlice = desc.m_firstMip;
            uavDesc.Texture2DArray.FirstArraySlice = desc.m_firstArray;
            uavDesc.Texture2DArray.ArraySize = desc.m_arrayCount;
            break;
        
        case D3D11_UAV_DIMENSION_TEXTURE3D:
            uavDesc.Texture3D.MipSlice = desc.m_firstMip;
            uavDesc.Texture3D.FirstWSlice = desc.m_firstDepth;
            uavDesc.Texture3D.WSize = desc.m_depthCount;
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
        if (buffer.GetBufferDesc().m_bufferSubType == BufferSubType::None)
        {
            DX_LOG(Error, "ShaderRWResourceView", "Unexpected Buffer subtype None in Shader RW Resource View.");
        }
        else if (buffer.GetBufferDesc().m_bufferSubType == BufferSubType::Structured &&
            desc.m_viewFormat != ResourceFormat::Unknown)
        {
            DX_LOG(Error, "ShaderRWResourceView", "Structured buffer only supports Unknown view format in Shader RW Resource View.");
        }
        else if (buffer.GetBufferDesc().m_bufferSubType == BufferSubType::Raw &&
            desc.m_viewFormat != ResourceFormat::R32_TYPELESS)
        {
            DX_LOG(Error, "ShaderRWResourceView", "Raw buffer only supports R32_TYPELESS view format in Shader RW Resource View.");
        }

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = ToDX11ResourceFormat(desc.m_viewFormat);
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

        // View boundaries to the buffer
        uavDesc.Buffer.FirstElement = desc.m_firstElement;
        uavDesc.Buffer.NumElements = desc.m_elementCount;
        uavDesc.Buffer.Flags = 0;
        if (buffer.GetBufferDesc().m_bufferSubType == BufferSubType::Raw)
        {
            uavDesc.Buffer.Flags |= D3D11_BUFFER_UAV_FLAG_RAW;
        }

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

            if (!(texture->get()->GetTextureDesc().m_bindFlags & TextureBind_ShaderRWResource))
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Shader RW Resource View description with texture that doesn't have the shader RW resource flag.");
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

            if (!(buffer->get()->GetBufferDesc().m_bindFlags & BufferBind_ShaderRWResource))
            {
                DX_LOG(Fatal, "ShaderRWResourceView", "Shader RW Resource View description with buffer that doesn't have the shader RW resource flag.");
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

    ComPtr<ID3D11UnorderedAccessView> ShaderRWResourceView::GetDX11UnorderedAccessView()
    {
        return m_dx11UnorderedAccessView;
    }
} // namespace DX
