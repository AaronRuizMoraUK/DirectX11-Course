#pragma once

#include <RHI/Shader/ShaderEnums.h>

#include <cstdint>

namespace DX
{
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
