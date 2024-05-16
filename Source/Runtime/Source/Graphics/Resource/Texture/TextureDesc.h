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
        Texture3D,

        Count
    };

    struct TextureDesc
    {
        TextureVariant m_variant;
        mathfu::Vector3Int m_size; // Dimensions of the texture.
        uint32_t m_mipLevels; // 0 to generate all mipmap levels. Use 1 for no mipmaps.
        ResourceFormat m_format;
        ResourceUsage m_usage;
        ResourceBindFlag m_bindFlag;
        ResourceCPUAccess m_cpuAccess;

        // Texture1D and Texture2D only. 0 to not use array, avoid using 1 as it'll expect a texture array of size 1.
        uint32_t m_arraySize;

        // The default sampler mode, with no anti - aliasing, has a count of 1 and a quality level of 0.
        uint32_t m_sampleCount; // Texture2D only
        uint32_t m_sampleQuality; // Texture2D only

        // When true the native resource is passed in m_data
        // When false the data is copied into the native texture.
        bool m_dataIsNativeResource;
        void* m_data;
    };
} // namespace DX
