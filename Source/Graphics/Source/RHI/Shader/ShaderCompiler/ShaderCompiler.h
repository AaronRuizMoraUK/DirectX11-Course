#pragma once

#include <RHI/Shader/ShaderEnums.h>
#include <RHI/Shader/ShaderBytecode.h>

#include <memory>

namespace DX
{
    class ShaderCompiler
    {
    public:
        // The shader name inside ShaderInfo is the filename relative to Assets folder.
        static std::shared_ptr<ShaderBytecode> Compile(const ShaderInfo& shaderInfo);
    };
} // namespace DX
