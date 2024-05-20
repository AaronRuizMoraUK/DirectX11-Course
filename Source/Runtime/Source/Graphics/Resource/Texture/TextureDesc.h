#pragma once

#include <Graphics/Resource/Texture/TextureEnums.h>
#include <Graphics/Resource/ResourceEnums.h>
#include <Math/Vector3.h>

namespace DX
{
    struct TextureDesc
    {
        TextureType m_textureType;
        Math::Vector3Int m_dimensions;
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
        const void* m_initialData;
    };
} // namespace DX
