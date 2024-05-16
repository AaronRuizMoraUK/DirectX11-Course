#pragma once

#include <Math/Vector3.h>
#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    enum class TextureVariant
    {
        Unknown = 0,

        Texture1D,
        Texture2D,
        TextureCube, // A two dimensional texture array with 6 sub-resources
        Texture3D,

        Count
    };

    //------------------------------------------------------------------------
    // Texture Format
    // 
    // Texture format is set explicitly inside the resource.
    // All its usages are views where the format must is specified too.
    // The texture format and the view format must be compatible.
    //------------------------------------------------------------------------

    // Bitwise operations on ResourceBindFlag are allowed.
    enum TextureBindFlag
    {
        TextureBind_ShaderResource = 1 << 0,
        TextureBind_ShaderRWResource = 1 << 1,
        TextureBind_RenderTarget = 1 << 2,
        TextureBind_DepthStencil = 1 << 3,
    };

    struct TextureDesc
    {
        TextureVariant m_variant;
        mathfu::Vector3Int m_size; // Dimensions of the texture.
        uint32_t m_mipLevels; // 0 to generate all mipmap levels. Use 1 for no mipmaps.
        ResourceFormat m_format;
        ResourceUsage m_usage;
        TextureBindFlag m_bindFlag;
        ResourceCPUAccess m_cpuAccess;

        // With Texture1D, Texture2D: 0 to not be a texture array, > 0 to be an array. Avoid using 1 as it'll expect a texture array of size 1.
        // With TextureCube: 6 to not be a texture array, multiples of 6 to be an array.
        uint32_t m_arraySize;

        // Texture2D only. The default sampler mode, with no multisampling, has a count of 1 and a quality level of 0.
        uint32_t m_sampleCount; // Texture2D only
        uint32_t m_sampleQuality; // Texture2D only

        // When true the native resource is passed in the initial data.
        // When false the initial data is copied into the native texture.
        bool m_initialDataIsNativeResource;
        void* m_initialData;
    };
} // namespace DX
