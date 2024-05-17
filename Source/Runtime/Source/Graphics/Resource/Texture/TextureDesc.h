#pragma once

#include <Math/Vector3.h>
#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    enum class TextureType
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

    enum TextureBindFlag
    {
        TextureBind_ShaderResource = 1 << 0,
        TextureBind_ShaderRWResource = 1 << 1,
        TextureBind_RenderTarget = 1 << 2,
        TextureBind_DepthStencil = 1 << 3,
    };

    namespace Internal
    {
        template<typename Tag>
        using TextureBindFlagsType = uint32_t;
    }
    using TextureBindFlags = Internal::TextureBindFlagsType<struct TextureBindFlagsTag>;


    struct TextureDesc
    {
        TextureType m_textureType;
        mathfu::Vector3Int m_dimensions;
        uint32_t m_mipCount; // 0 to generate all mipmap levels. Use 1 for no mipmaps.
        ResourceFormat m_format;
        ResourceUsage m_usage;
        TextureBindFlags m_bindFlags; // Bitwise operation of TextureBindFlag
        ResourceCPUAccess m_cpuAccess;

        // With Texture1D, Texture2D: 1 to not be a texture array, > 1 to be an array.
        // With TextureCube: 6 to not be a texture array, multiples of 6 to be an array.
        uint32_t m_arrayCount;

        // Texture2D only. The default sampler mode, with no multisampling, has a count of 1 and a quality level of 0.
        uint32_t m_sampleCount; // Texture2D only
        uint32_t m_sampleQuality; // Texture2D only

        // When true the native resource is passed in the initial data.
        // When false the initial data is copied into the native texture.
        bool m_initialDataIsNativeResource;
        void* m_initialData;
    };
} // namespace DX
