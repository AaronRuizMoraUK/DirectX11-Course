#pragma once

#include <RHI/Shader/ShaderEnums.h>

#include <memory>

namespace DX
{
    class ShaderBytecode;

    struct ShaderDesc
    {
        ShaderInfo m_shaderInfo;

        std::shared_ptr<const ShaderBytecode> m_bytecode;
    };
} // namespace DX
