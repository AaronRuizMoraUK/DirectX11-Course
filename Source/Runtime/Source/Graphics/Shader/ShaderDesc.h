#pragma once

namespace DX
{
    enum class ShaderType
    {
        Unknown = 0,

        Vertex,
        Pixel,
        Geometry,
        Hull,
        Domain,
        Compute,

        Count
    };

    struct ShaderDesc
    {
        ShaderType m_shaderType;
    };
} // namespace DX
