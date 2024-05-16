#pragma once

#include <Graphics/Shader/ShaderDesc.h>
#include <Graphics/Shader/ShaderBytecode.h>

#include <memory>

namespace DX
{
    class ShaderCompiler
    {
    public:
        static std::unique_ptr<ShaderBytecode> Compile(const ShaderInfo& shaderInfo);
    };
} // namespace DX
