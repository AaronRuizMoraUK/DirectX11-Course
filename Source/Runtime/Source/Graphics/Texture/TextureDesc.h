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
        Texture3D,

        Count
    };

    struct TextureDesc
    {
        TextureType m_type;
        mathfu::Vector3Int m_size;
        uint32_t m_mipLevels;
        uint32_t m_arraySize;     // Texture1D and Texture2D only
        ResourceFormat m_format;
        uint32_t m_sampleCount;   // Texture2D only
        uint32_t m_sampleQuality; // Texture2D only
        void* m_data;
    };
} // namespace DX
