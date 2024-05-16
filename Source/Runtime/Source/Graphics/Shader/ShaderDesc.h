#pragma once

#include <cstdint>

namespace DX
{
    enum class ShaderType
    {
        Unknown = 0,

        Vertex,
        Hull,
        Domain,
        Geometry,
        Pixel,
        Compute,

        Count
    };

    struct ShaderDesc
    {
        ShaderType m_shaderType;

        void* m_bytecode;
        uint32_t m_bytecodeSizeInBytes;
    };
} // namespace DX
