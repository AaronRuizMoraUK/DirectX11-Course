#pragma once

#include <Graphics/Shader/ShaderEnums.h>

#include <memory>

namespace DX
{
    class ShaderBytecode;

    struct ShaderDesc
    {
        ShaderInfo m_shaderInfo;

        std::shared_ptr<ShaderBytecode> m_bytecode;
    };
} // namespace DX
