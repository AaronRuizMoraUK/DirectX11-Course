#pragma once

#include <string>

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

    struct ShaderInfo
    {
        ShaderType m_shaderType;
        std::string m_name;
        std::string m_entryPoint;
    };
} // namespace DX
