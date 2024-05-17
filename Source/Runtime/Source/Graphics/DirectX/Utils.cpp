#include <Graphics/DirectX/Utils.h>

#include <Log/Log.h>

namespace DX
{
    D3D11_USAGE ToDX11ResourceUsage(ResourceUsage usage)
    {
        switch (usage)
        {
        case ResourceUsage::Default:   return D3D11_USAGE_DEFAULT;
        case ResourceUsage::Immutable: return D3D11_USAGE_IMMUTABLE;
        case ResourceUsage::Dynamic:   return D3D11_USAGE_DYNAMIC;
        case ResourceUsage::Staging:   return D3D11_USAGE_STAGING;

        case ResourceUsage::Unknown:
        default:
            DX_LOG(Error, "Utils", "Unknown resource usage value %d", usage);
            return D3D11_USAGE_DEFAULT;
        }
    }

    uint32_t ToDX11TextureBindFlags(TextureBindFlags bindFlag)
    {
        uint32_t dx11BindFlags = 0;

        dx11BindFlags |= (bindFlag & TextureBind_ShaderResource)   ? D3D11_BIND_SHADER_RESOURCE  : 0;
        dx11BindFlags |= (bindFlag & TextureBind_ShaderRWResource) ? D3D11_BIND_UNORDERED_ACCESS : 0;
        dx11BindFlags |= (bindFlag & TextureBind_RenderTarget)     ? D3D11_BIND_RENDER_TARGET    : 0;
        dx11BindFlags |= (bindFlag & TextureBind_DepthStencil)     ? D3D11_BIND_DEPTH_STENCIL    : 0;

        return dx11BindFlags;
    }

    uint32_t ToDX11BufferBindFlags(BufferBindFlags bindFlag)
    {
        uint32_t dx11BindFlags = 0;

        dx11BindFlags |= (bindFlag & BufferBind_VertexBuffer) ? D3D11_BIND_VERTEX_BUFFER : 0;
        dx11BindFlags |= (bindFlag & BufferBind_IndexBuffer) ? D3D11_BIND_INDEX_BUFFER : 0;
        dx11BindFlags |= (bindFlag & BufferBind_ConstantBuffer) ? D3D11_BIND_CONSTANT_BUFFER : 0;
        dx11BindFlags |= (bindFlag & BufferBind_ShaderResource) ? D3D11_BIND_SHADER_RESOURCE : 0;
        dx11BindFlags |= (bindFlag & BufferBind_ShaderRWResource) ? D3D11_BIND_UNORDERED_ACCESS : 0;
        dx11BindFlags |= (bindFlag & BufferBind_RenderTarget) ? D3D11_BIND_RENDER_TARGET : 0;
        dx11BindFlags |= (bindFlag & BufferBind_StreamOutput) ? D3D11_BIND_STREAM_OUTPUT : 0;

        return dx11BindFlags;
    }

    uint32_t ToDX11ResourceCPUAccess(ResourceCPUAccess cpuAccess)
    {
        switch (cpuAccess)
        {
        case ResourceCPUAccess::None:      return 0;
        case ResourceCPUAccess::Read:      return D3D11_CPU_ACCESS_READ;
        case ResourceCPUAccess::Write:     return D3D11_CPU_ACCESS_WRITE;
        case ResourceCPUAccess::ReadWrite: return D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

        default:
            DX_LOG(Error, "Utils", "Unknown resource CPU access value %d", cpuAccess);
            return 0;
        }
    }

    DXGI_FORMAT ToDX11ResourceFormat(ResourceFormat format)
    {
        switch (format)
        {
        case ResourceFormat::Unknown:                     return DXGI_FORMAT_UNKNOWN;

        case ResourceFormat::R32G32B32A32_TYPELESS:       return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case ResourceFormat::R32G32B32A32_FLOAT:          return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ResourceFormat::R32G32B32A32_UINT:           return DXGI_FORMAT_R32G32B32A32_UINT;
        case ResourceFormat::R32G32B32A32_SINT:           return DXGI_FORMAT_R32G32B32A32_SINT;

        case ResourceFormat::R32G32B32_TYPELESS:          return DXGI_FORMAT_R32G32B32_TYPELESS;
        case ResourceFormat::R32G32B32_FLOAT:             return DXGI_FORMAT_R32G32B32_FLOAT;
        case ResourceFormat::R32G32B32_UINT:              return DXGI_FORMAT_R32G32B32_UINT;
        case ResourceFormat::R32G32B32_SINT:              return DXGI_FORMAT_R32G32B32_SINT;

        case ResourceFormat::R16G16B16A16_TYPELESS:       return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case ResourceFormat::R16G16B16A16_FLOAT:          return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case ResourceFormat::R16G16B16A16_UNORM:          return DXGI_FORMAT_R16G16B16A16_UNORM;
        case ResourceFormat::R16G16B16A16_UINT:           return DXGI_FORMAT_R16G16B16A16_UINT;
        case ResourceFormat::R16G16B16A16_SNORM:          return DXGI_FORMAT_R16G16B16A16_SNORM;
        case ResourceFormat::R16G16B16A16_SINT:           return DXGI_FORMAT_R16G16B16A16_SINT;

        case ResourceFormat::R32G32_TYPELESS:             return DXGI_FORMAT_R32G32_TYPELESS;
        case ResourceFormat::R32G32_FLOAT:                return DXGI_FORMAT_R32G32_FLOAT;
        case ResourceFormat::R32G32_UINT:                 return DXGI_FORMAT_R32G32_UINT;
        case ResourceFormat::R32G32_SINT:                 return DXGI_FORMAT_R32G32_SINT;
        case ResourceFormat::R32G8X24_TYPELESS:           return DXGI_FORMAT_R32G8X24_TYPELESS;

        case ResourceFormat::D32_FLOAT_S8X24_UINT:        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case ResourceFormat::R32_FLOAT_X8X24_TYPELESS:    return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        case ResourceFormat::X32_TYPELESS_G8X24_UINT:     return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

        case ResourceFormat::R10G10B10A2_TYPELESS:        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        case ResourceFormat::R10G10B10A2_UNORM:           return DXGI_FORMAT_R10G10B10A2_UNORM;
        case ResourceFormat::R10G10B10A2_UINT:            return DXGI_FORMAT_R10G10B10A2_UINT;
        case ResourceFormat::R11G11B10_FLOAT:             return DXGI_FORMAT_R11G11B10_FLOAT;

        case ResourceFormat::R8G8B8A8_TYPELESS:           return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case ResourceFormat::R8G8B8A8_UNORM:              return DXGI_FORMAT_R8G8B8A8_UNORM;
        case ResourceFormat::R8G8B8A8_UNORM_SRGB:         return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case ResourceFormat::R8G8B8A8_UINT:               return DXGI_FORMAT_R8G8B8A8_UINT;
        case ResourceFormat::R8G8B8A8_SNORM:              return DXGI_FORMAT_R8G8B8A8_SNORM;
        case ResourceFormat::R8G8B8A8_SINT:               return DXGI_FORMAT_R8G8B8A8_SINT;

        case ResourceFormat::R16G16_TYPELESS:             return DXGI_FORMAT_R16G16_TYPELESS;
        case ResourceFormat::R16G16_FLOAT:                return DXGI_FORMAT_R16G16_FLOAT;
        case ResourceFormat::R16G16_UNORM:                return DXGI_FORMAT_R16G16_UNORM;
        case ResourceFormat::R16G16_UINT:                 return DXGI_FORMAT_R16G16_UINT;
        case ResourceFormat::R16G16_SNORM:                return DXGI_FORMAT_R16G16_SNORM;
        case ResourceFormat::R16G16_SINT:                 return DXGI_FORMAT_R16G16_SINT;

        case ResourceFormat::R32_TYPELESS:                return DXGI_FORMAT_R32_TYPELESS;
        case ResourceFormat::D32_FLOAT:                   return DXGI_FORMAT_D32_FLOAT;
        case ResourceFormat::R32_FLOAT:                   return DXGI_FORMAT_R32_FLOAT;
        case ResourceFormat::R32_UINT:                    return DXGI_FORMAT_R32_UINT;
        case ResourceFormat::R32_SINT:                    return DXGI_FORMAT_R32_SINT;

        case ResourceFormat::R24G8_TYPELESS:              return DXGI_FORMAT_R24G8_TYPELESS;
        case ResourceFormat::D24_UNORM_S8_UINT:           return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case ResourceFormat::R24_UNORM_X8_TYPELESS:       return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case ResourceFormat::X24_TYPELESS_G8_UINT:        return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

        case ResourceFormat::R8G8_TYPELESS:               return DXGI_FORMAT_R8G8_TYPELESS;
        case ResourceFormat::R8G8_UNORM:                  return DXGI_FORMAT_R8G8_UNORM;
        case ResourceFormat::R8G8_UINT:                   return DXGI_FORMAT_R8G8_UINT;
        case ResourceFormat::R8G8_SNORM:                  return DXGI_FORMAT_R8G8_SNORM;
        case ResourceFormat::R8G8_SINT:                   return DXGI_FORMAT_R8G8_SINT;

        case ResourceFormat::R16_TYPELESS:                return DXGI_FORMAT_R16_TYPELESS;
        case ResourceFormat::R16_FLOAT:                   return DXGI_FORMAT_R16_FLOAT;
        case ResourceFormat::D16_UNORM:                   return DXGI_FORMAT_D16_UNORM;
        case ResourceFormat::R16_UNORM:                   return DXGI_FORMAT_R16_UNORM;
        case ResourceFormat::R16_UINT:                    return DXGI_FORMAT_R16_UINT;
        case ResourceFormat::R16_SNORM:                   return DXGI_FORMAT_R16_SNORM;
        case ResourceFormat::R16_SINT:                    return DXGI_FORMAT_R16_SINT;

        case ResourceFormat::R8_TYPELESS:                 return DXGI_FORMAT_R8_TYPELESS;
        case ResourceFormat::R8_UNORM:                    return DXGI_FORMAT_R8_UNORM;
        case ResourceFormat::R8_UINT:                     return DXGI_FORMAT_R8_UINT;
        case ResourceFormat::R8_SNORM:                    return DXGI_FORMAT_R8_SNORM;
        case ResourceFormat::R8_SINT:                     return DXGI_FORMAT_R8_SINT;
        case ResourceFormat::A8_UNORM:                    return DXGI_FORMAT_A8_UNORM;
        case ResourceFormat::R1_UNORM:                    return DXGI_FORMAT_R1_UNORM;

        case ResourceFormat::R9G9B9E5_SHAREDEXP:          return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case ResourceFormat::R8G8_B8G8_UNORM:             return DXGI_FORMAT_R8G8_B8G8_UNORM;
        case ResourceFormat::G8R8_G8B8_UNORM:             return DXGI_FORMAT_G8R8_G8B8_UNORM;

        case ResourceFormat::BC1_TYPELESS:                return DXGI_FORMAT_BC1_TYPELESS;
        case ResourceFormat::BC1_UNORM:                   return DXGI_FORMAT_BC1_UNORM;
        case ResourceFormat::BC1_UNORM_SRGB:              return DXGI_FORMAT_BC1_UNORM_SRGB;
        case ResourceFormat::BC2_TYPELESS:                return DXGI_FORMAT_BC2_TYPELESS;
        case ResourceFormat::BC2_UNORM:                   return DXGI_FORMAT_BC2_UNORM;
        case ResourceFormat::BC2_UNORM_SRGB:              return DXGI_FORMAT_BC2_UNORM_SRGB;
        case ResourceFormat::BC3_TYPELESS:                return DXGI_FORMAT_BC3_TYPELESS;
        case ResourceFormat::BC3_UNORM:                   return DXGI_FORMAT_BC3_UNORM;
        case ResourceFormat::BC3_UNORM_SRGB:              return DXGI_FORMAT_BC3_UNORM_SRGB;
        case ResourceFormat::BC4_TYPELESS:                return DXGI_FORMAT_BC4_TYPELESS;
        case ResourceFormat::BC4_UNORM:                   return DXGI_FORMAT_BC4_UNORM;
        case ResourceFormat::BC4_SNORM:                   return DXGI_FORMAT_BC4_SNORM;
        case ResourceFormat::BC5_TYPELESS:                return DXGI_FORMAT_BC5_TYPELESS;
        case ResourceFormat::BC5_UNORM:                   return DXGI_FORMAT_BC5_UNORM;
        case ResourceFormat::BC5_SNORM:                   return DXGI_FORMAT_BC5_SNORM;

        case ResourceFormat::B5G6R5_UNORM:                return DXGI_FORMAT_B5G6R5_UNORM;
        case ResourceFormat::B5G5R5A1_UNORM:              return DXGI_FORMAT_B5G5R5A1_UNORM;
        case ResourceFormat::B8G8R8A8_UNORM:              return DXGI_FORMAT_B8G8R8A8_UNORM;
        case ResourceFormat::B8G8R8X8_UNORM:              return DXGI_FORMAT_B8G8R8X8_UNORM;
        case ResourceFormat::R10G10B10_XR_BIAS_A2_UNORM:  return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        case ResourceFormat::B8G8R8A8_TYPELESS:           return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        case ResourceFormat::B8G8R8A8_UNORM_SRGB:         return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case ResourceFormat::B8G8R8X8_TYPELESS:           return DXGI_FORMAT_B8G8R8X8_TYPELESS;
        case ResourceFormat::B8G8R8X8_UNORM_SRGB:         return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

        case ResourceFormat::BC6H_TYPELESS:               return DXGI_FORMAT_BC6H_TYPELESS;
        case ResourceFormat::BC6H_UF16:                   return DXGI_FORMAT_BC6H_UF16;
        case ResourceFormat::BC6H_SF16:                   return DXGI_FORMAT_BC6H_SF16;
        case ResourceFormat::BC7_TYPELESS:                return DXGI_FORMAT_BC7_TYPELESS;
        case ResourceFormat::BC7_UNORM:                   return DXGI_FORMAT_BC7_UNORM;
        case ResourceFormat::BC7_UNORM_SRGB:              return DXGI_FORMAT_BC7_UNORM_SRGB;

        default:
            DX_LOG(Error, "Utils", "Unknown resource format %d", format);
            return DXGI_FORMAT_UNKNOWN;
        }
    }
}
