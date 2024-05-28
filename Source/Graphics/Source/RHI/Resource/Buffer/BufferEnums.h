#pragma once

#include <cstdint>

namespace DX
{
    // Subtype of buffer when used in a shader or render target.
    enum class BufferSubType
    {
        None = 0, // Not a buffer for a shader or render target.
        Typed, // Buffer where each element is a basic types: floatX, intX or uintX.
        Structured, // Buffer where each element is a struct. Format must be Unknown. Not supported with render targets.
        Raw, // Buffer where each element is 4 bytes. Format must R32_TYPELESS. Not supported with render targets.

        Total
    };

    const char* BufferSubTypeStr(BufferSubType bufferSubType);

    // Bitwise operations on ResourceBindFlag are allowed.
    enum BufferBindFlag
    {
        BufferBind_VertexBuffer = 1 << 0,
        BufferBind_IndexBuffer = 1 << 1,
        BufferBind_ConstantBuffer = 1 << 2,
        BufferBind_ShaderResource = 1 << 3,
        BufferBind_ShaderRWResource = 1 << 4,
        BufferBind_RenderTarget = 1 << 5,
        BufferBind_StreamOutput = 1 << 6,
    };

    using BufferBindFlags = uint32_t;

} // namespace DX
