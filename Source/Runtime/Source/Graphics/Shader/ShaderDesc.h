#pragma once

#include <Graphics/Shader/ShaderFlags.h>

#include <string>

namespace DX
{
    class ShaderBytecode;

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
