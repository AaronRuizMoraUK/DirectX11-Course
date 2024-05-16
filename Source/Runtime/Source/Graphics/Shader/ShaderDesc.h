#pragma once

#include <string>

namespace DX
{
    class ShaderBytecode;

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

    struct ShaderDesc
    {
        ShaderInfo m_shaderInfo;

        ShaderBytecode* m_bytecode;
    };
} // namespace DX
