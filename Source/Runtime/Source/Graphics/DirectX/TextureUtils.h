#pragma once

#include <Log/Log.h>
#include <Graphics/Texture/TextureFormat.h>

#include <d3d11.h>

namespace DX
{
    static DXGI_FORMAT ToDX11TextureFormat(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Unknown:                     return DXGI_FORMAT_UNKNOWN;

        case TextureFormat::R32G32B32A32_TYPELESS:       return DXGI_FORMAT_R32G32B32A32_TYPELESS;
        case TextureFormat::R32G32B32A32_FLOAT:          return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case TextureFormat::R32G32B32A32_UINT:           return DXGI_FORMAT_R32G32B32A32_UINT;
        case TextureFormat::R32G32B32A32_SINT:           return DXGI_FORMAT_R32G32B32A32_SINT;

        case TextureFormat::R32G32B32_TYPELESS:          return DXGI_FORMAT_R32G32B32_TYPELESS;
        case TextureFormat::R32G32B32_FLOAT:             return DXGI_FORMAT_R32G32B32_FLOAT;
        case TextureFormat::R32G32B32_UINT:              return DXGI_FORMAT_R32G32B32_UINT;
        case TextureFormat::R32G32B32_SINT:              return DXGI_FORMAT_R32G32B32_SINT;

        case TextureFormat::R16G16B16A16_TYPELESS:       return DXGI_FORMAT_R16G16B16A16_TYPELESS;
        case TextureFormat::R16G16B16A16_FLOAT:          return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case TextureFormat::R16G16B16A16_UNORM:          return DXGI_FORMAT_R16G16B16A16_UNORM;
        case TextureFormat::R16G16B16A16_UINT:           return DXGI_FORMAT_R16G16B16A16_UINT;
        case TextureFormat::R16G16B16A16_SNORM:          return DXGI_FORMAT_R16G16B16A16_SNORM;
        case TextureFormat::R16G16B16A16_SINT:           return DXGI_FORMAT_R16G16B16A16_SINT;

        case TextureFormat::R32G32_TYPELESS:             return DXGI_FORMAT_R32G32_TYPELESS;
        case TextureFormat::R32G32_FLOAT:                return DXGI_FORMAT_R32G32_FLOAT;
        case TextureFormat::R32G32_UINT:                 return DXGI_FORMAT_R32G32_UINT;
        case TextureFormat::R32G32_SINT:                 return DXGI_FORMAT_R32G32_SINT;
        case TextureFormat::R32G8X24_TYPELESS:           return DXGI_FORMAT_R32G8X24_TYPELESS;

        case TextureFormat::D32_FLOAT_S8X24_UINT:        return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        case TextureFormat::R32_FLOAT_X8X24_TYPELESS:    return DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;
        case TextureFormat::X32_TYPELESS_G8X24_UINT:     return DXGI_FORMAT_X32_TYPELESS_G8X24_UINT;

        case TextureFormat::R10G10B10A2_TYPELESS:        return DXGI_FORMAT_R10G10B10A2_TYPELESS;
        case TextureFormat::R10G10B10A2_UNORM:           return DXGI_FORMAT_R10G10B10A2_UNORM;
        case TextureFormat::R10G10B10A2_UINT:            return DXGI_FORMAT_R10G10B10A2_UINT;
        case TextureFormat::R11G11B10_FLOAT:             return DXGI_FORMAT_R11G11B10_FLOAT;

        case TextureFormat::R8G8B8A8_TYPELESS:           return DXGI_FORMAT_R8G8B8A8_TYPELESS;
        case TextureFormat::R8G8B8A8_UNORM:              return DXGI_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::R8G8B8A8_UNORM_SRGB:         return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        case TextureFormat::R8G8B8A8_UINT:               return DXGI_FORMAT_R8G8B8A8_UINT;
        case TextureFormat::R8G8B8A8_SNORM:              return DXGI_FORMAT_R8G8B8A8_SNORM;
        case TextureFormat::R8G8B8A8_SINT:               return DXGI_FORMAT_R8G8B8A8_SINT;

        case TextureFormat::R16G16_TYPELESS:             return DXGI_FORMAT_R16G16_TYPELESS;
        case TextureFormat::R16G16_FLOAT:                return DXGI_FORMAT_R16G16_FLOAT;
        case TextureFormat::R16G16_UNORM:                return DXGI_FORMAT_R16G16_UNORM;
        case TextureFormat::R16G16_UINT:                 return DXGI_FORMAT_R16G16_UINT;
        case TextureFormat::R16G16_SNORM:                return DXGI_FORMAT_R16G16_SNORM;
        case TextureFormat::R16G16_SINT:                 return DXGI_FORMAT_R16G16_SINT;

        case TextureFormat::R32_TYPELESS:                return DXGI_FORMAT_R32_TYPELESS;
        case TextureFormat::D32_FLOAT:                   return DXGI_FORMAT_D32_FLOAT;
        case TextureFormat::R32_FLOAT:                   return DXGI_FORMAT_R32_FLOAT;
        case TextureFormat::R32_UINT:                    return DXGI_FORMAT_R32_UINT;
        case TextureFormat::R32_SINT:                    return DXGI_FORMAT_R32_SINT;

        case TextureFormat::R24G8_TYPELESS:              return DXGI_FORMAT_R24G8_TYPELESS;
        case TextureFormat::D24_UNORM_S8_UINT:           return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormat::R24_UNORM_X8_TYPELESS:       return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        case TextureFormat::X24_TYPELESS_G8_UINT:        return DXGI_FORMAT_X24_TYPELESS_G8_UINT;

        case TextureFormat::R8G8_TYPELESS:               return DXGI_FORMAT_R8G8_TYPELESS;
        case TextureFormat::R8G8_UNORM:                  return DXGI_FORMAT_R8G8_UNORM;
        case TextureFormat::R8G8_UINT:                   return DXGI_FORMAT_R8G8_UINT;
        case TextureFormat::R8G8_SNORM:                  return DXGI_FORMAT_R8G8_SNORM;
        case TextureFormat::R8G8_SINT:                   return DXGI_FORMAT_R8G8_SINT;

        case TextureFormat::R16_TYPELESS:                return DXGI_FORMAT_R16_TYPELESS;
        case TextureFormat::R16_FLOAT:                   return DXGI_FORMAT_R16_FLOAT;
        case TextureFormat::D16_UNORM:                   return DXGI_FORMAT_D16_UNORM;
        case TextureFormat::R16_UNORM:                   return DXGI_FORMAT_R16_UNORM;
        case TextureFormat::R16_UINT:                    return DXGI_FORMAT_R16_UINT;
        case TextureFormat::R16_SNORM:                   return DXGI_FORMAT_R16_SNORM;
        case TextureFormat::R16_SINT:                    return DXGI_FORMAT_R16_SINT;

        case TextureFormat::R8_TYPELESS:                 return DXGI_FORMAT_R8_TYPELESS;
        case TextureFormat::R8_UNORM:                    return DXGI_FORMAT_R8_UNORM;
        case TextureFormat::R8_UINT:                     return DXGI_FORMAT_R8_UINT;
        case TextureFormat::R8_SNORM:                    return DXGI_FORMAT_R8_SNORM;
        case TextureFormat::R8_SINT:                     return DXGI_FORMAT_R8_SINT;
        case TextureFormat::A8_UNORM:                    return DXGI_FORMAT_A8_UNORM;
        case TextureFormat::R1_UNORM:                    return DXGI_FORMAT_R1_UNORM;

        case TextureFormat::R9G9B9E5_SHAREDEXP:          return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
        case TextureFormat::R8G8_B8G8_UNORM:             return DXGI_FORMAT_R8G8_B8G8_UNORM;
        case TextureFormat::G8R8_G8B8_UNORM:             return DXGI_FORMAT_G8R8_G8B8_UNORM;

        case TextureFormat::BC1_TYPELESS:                return DXGI_FORMAT_BC1_TYPELESS;
        case TextureFormat::BC1_UNORM:                   return DXGI_FORMAT_BC1_UNORM;
        case TextureFormat::BC1_UNORM_SRGB:              return DXGI_FORMAT_BC1_UNORM_SRGB;
        case TextureFormat::BC2_TYPELESS:                return DXGI_FORMAT_BC2_TYPELESS;
        case TextureFormat::BC2_UNORM:                   return DXGI_FORMAT_BC2_UNORM;
        case TextureFormat::BC2_UNORM_SRGB:              return DXGI_FORMAT_BC2_UNORM_SRGB;
        case TextureFormat::BC3_TYPELESS:                return DXGI_FORMAT_BC3_TYPELESS;
        case TextureFormat::BC3_UNORM:                   return DXGI_FORMAT_BC3_UNORM;
        case TextureFormat::BC3_UNORM_SRGB:              return DXGI_FORMAT_BC3_UNORM_SRGB;
        case TextureFormat::BC4_TYPELESS:                return DXGI_FORMAT_BC4_TYPELESS;
        case TextureFormat::BC4_UNORM:                   return DXGI_FORMAT_BC4_UNORM;
        case TextureFormat::BC4_SNORM:                   return DXGI_FORMAT_BC4_SNORM;
        case TextureFormat::BC5_TYPELESS:                return DXGI_FORMAT_BC5_TYPELESS;
        case TextureFormat::BC5_UNORM:                   return DXGI_FORMAT_BC5_UNORM;
        case TextureFormat::BC5_SNORM:                   return DXGI_FORMAT_BC5_SNORM;

        case TextureFormat::B5G6R5_UNORM:                return DXGI_FORMAT_B5G6R5_UNORM;
        case TextureFormat::B5G5R5A1_UNORM:              return DXGI_FORMAT_B5G5R5A1_UNORM;
        case TextureFormat::B8G8R8A8_UNORM:              return DXGI_FORMAT_B8G8R8A8_UNORM;
        case TextureFormat::B8G8R8X8_UNORM:              return DXGI_FORMAT_B8G8R8X8_UNORM;
        case TextureFormat::R10G10B10_XR_BIAS_A2_UNORM:  return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
        case TextureFormat::B8G8R8A8_TYPELESS:           return DXGI_FORMAT_B8G8R8A8_TYPELESS;
        case TextureFormat::B8G8R8A8_UNORM_SRGB:         return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
        case TextureFormat::B8G8R8X8_TYPELESS:           return DXGI_FORMAT_B8G8R8X8_TYPELESS;
        case TextureFormat::B8G8R8X8_UNORM_SRGB:         return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

        case TextureFormat::BC6H_TYPELESS:               return DXGI_FORMAT_BC6H_TYPELESS;
        case TextureFormat::BC6H_UF16:                   return DXGI_FORMAT_BC6H_UF16;
        case TextureFormat::BC6H_SF16:                   return DXGI_FORMAT_BC6H_SF16;
        case TextureFormat::BC7_TYPELESS:                return DXGI_FORMAT_BC7_TYPELESS;
        case TextureFormat::BC7_UNORM:                   return DXGI_FORMAT_BC7_UNORM;
        case TextureFormat::BC7_UNORM_SRGB:              return DXGI_FORMAT_BC7_UNORM_SRGB;

        default:
            DX_LOG(Error, "TextureUtils", "Unknown texture format %d", format);
            return DXGI_FORMAT_UNKNOWN;
        }
    }
}