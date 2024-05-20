#pragma once

#include <cstdint>

namespace DX
{
    enum class ResourceUsage
    {
        Unknown = 0,

        Default,
        Immutable, // Immutable resources are not expected to be updated. Its data is only set when creating the resource.
        Dynamic,
        Staging,

        Count
    };

    enum class ResourceCPUAccess
    {
        None = 0,
        Read,
        Write,
        ReadWrite,

        Count
    };

    enum class ComparisonFunction
    {
        Unknown = 0,

        Never,
        Less,
        Equal,
        LessEqual,
        Greater,
        NotEqual,
        GreaterEqual,
        Always,

        Count
    };

    enum class ResourceFormat
    {
        Unknown = 0,

        R32G32B32A32_TYPELESS,
        R32G32B32A32_FLOAT,
        R32G32B32A32_UINT,
        R32G32B32A32_SINT,

        R32G32B32_TYPELESS,
        R32G32B32_FLOAT,
        R32G32B32_UINT,
        R32G32B32_SINT,

        R16G16B16A16_TYPELESS,
        R16G16B16A16_FLOAT,
        R16G16B16A16_UNORM,
        R16G16B16A16_UINT,
        R16G16B16A16_SNORM,
        R16G16B16A16_SINT,

        R32G32_TYPELESS,
        R32G32_FLOAT,
        R32G32_UINT,
        R32G32_SINT,
        R32G8X24_TYPELESS,

        D32_FLOAT_S8X24_UINT,
        R32_FLOAT_X8X24_TYPELESS,
        X32_TYPELESS_G8X24_UINT,

        R10G10B10A2_TYPELESS,
        R10G10B10A2_UNORM,
        R10G10B10A2_UINT,
        R11G11B10_FLOAT,

        R8G8B8A8_TYPELESS,
        R8G8B8A8_UNORM,
        R8G8B8A8_UNORM_SRGB,
        R8G8B8A8_UINT,
        R8G8B8A8_SNORM,
        R8G8B8A8_SINT,

        R16G16_TYPELESS,
        R16G16_FLOAT,
        R16G16_UNORM,
        R16G16_UINT,
        R16G16_SNORM,
        R16G16_SINT,

        R32_TYPELESS,
        D32_FLOAT,
        R32_FLOAT,
        R32_UINT,
        R32_SINT,

        R24G8_TYPELESS,
        D24_UNORM_S8_UINT,
        R24_UNORM_X8_TYPELESS,
        X24_TYPELESS_G8_UINT,

        R8G8_TYPELESS,
        R8G8_UNORM,
        R8G8_UINT,
        R8G8_SNORM,
        R8G8_SINT,

        R16_TYPELESS,
        R16_FLOAT,
        D16_UNORM,
        R16_UNORM,
        R16_UINT,
        R16_SNORM,
        R16_SINT,

        R8_TYPELESS,
        R8_UNORM,
        R8_UINT,
        R8_SNORM,
        R8_SINT,
        A8_UNORM,
        R1_UNORM,

        R9G9B9E5_SHAREDEXP,
        R8G8_B8G8_UNORM,
        G8R8_G8B8_UNORM,

        //  Also known as DXT1, this format is used for color maps and can handle 1 bit of alpha.
        BC1_TYPELESS,
        BC1_UNORM,
        BC1_UNORM_SRGB,

        // Also known as DXT3, this format includes explicit alpha and is used for textures with more detailed alpha transitions.
        BC2_TYPELESS,
        BC2_UNORM,
        BC2_UNORM_SRGB,

        // Also known as DXT5, it provides interpolated alpha and is preferred for better alpha gradients compared to BC2.
        BC3_TYPELESS,
        BC3_UNORM,
        BC3_UNORM_SRGB,

        // Used primarily for single-channel data (red only), such as height maps or alpha maps.
        BC4_TYPELESS,
        BC4_UNORM,
        BC4_SNORM,

        // Stores two channels (red and green), commonly used for normal maps or two-channel data maps.
        BC5_TYPELESS,
        BC5_UNORM,
        BC5_SNORM,

        B5G6R5_UNORM,
        B5G5R5A1_UNORM,
        B8G8R8A8_UNORM,
        B8G8R8X8_UNORM,
        R10G10B10_XR_BIAS_A2_UNORM,
        B8G8R8A8_TYPELESS,
        B8G8R8A8_UNORM_SRGB,
        B8G8R8X8_TYPELESS,
        B8G8R8X8_UNORM_SRGB,

        // Designed for high dynamic range (HDR) images
        BC6H_TYPELESS,
        BC6H_UF16,
        BC6H_SF16,

        // Offers a higher quality color compression, supporting a full range of alpha and more complex color palettes,
        // suitable for high quality texture compression.
        BC7_TYPELESS,
        BC7_UNORM,
        BC7_UNORM_SRGB,

        Count
    };

    int ResourceFormatSize(ResourceFormat format, int elementCount = 1);

    bool IsCompressedResourceFormat(ResourceFormat format);
} // namespace DX
