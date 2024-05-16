#pragma once

#include <cstdint>

namespace DX
{
    class ShaderBytecode
    {
    public:
        ShaderBytecode() = default;
        virtual ~ShaderBytecode() = default;

        virtual void* GetData() const = 0;
        virtual uint32_t GetSize() const = 0;
    };
} // namespace DX
