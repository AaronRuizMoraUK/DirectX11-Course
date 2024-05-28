#pragma once

#include <RHI/Shader/ShaderResourceLayout.h>

#include <cstdint>

namespace DX
{
    // Compiled bytecode for a shader.
    // It also has the list of resources that are used by the shader.
    class ShaderBytecode
    {
    public:
        ShaderBytecode(ShaderResourceLayout&& shaderResourceLayout)
            : m_shaderResourceLayout(std::move(shaderResourceLayout))
        {

        }

        virtual ~ShaderBytecode() = default;

        virtual const void* GetData() const = 0;
        virtual uint32_t GetSize() const = 0;

        const ShaderResourceLayout& GetShaderResourceLayout() const
        {
            return m_shaderResourceLayout;
        }

    protected:
        ShaderResourceLayout m_shaderResourceLayout;
    };
} // namespace DX
