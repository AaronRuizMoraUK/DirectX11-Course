#pragma once

#include <Graphics/Shader/ShaderFlags.h>

namespace DX
{
    class ShaderBytecode;

    struct ShaderDesc
    {
        ShaderInfo m_shaderInfo;

        ShaderBytecode* m_bytecode;
    };
} // namespace DX
