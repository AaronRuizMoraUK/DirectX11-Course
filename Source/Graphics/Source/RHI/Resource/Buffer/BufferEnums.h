#pragma once

#include <cstdint>

namespace DX
{
    //------------------------------------------------------------------------
    // Buffer Format
    // 
    // Buffer format is implicit and not set inside the description.
    // The format of the buffer is set later depending on what's used for (binding):
    // 
    // - As VertexBuffer: format is specified with an InputLayout, which is set with DeviceContext->IASetInputLayout() call.
    // - As IndexBuffer: format is specified in DeviceContext->IASetIndexBuffer() call.
    // - As ConstantBuffer: format is expected to match the structure of the constant buffer in the shader.
    // - As ShaderResource, ShaderRWResource or RenderTarget: format is specified in their respective view description.
    // - As StreamOutput: format is always 32-bit values coming from the output declarations of the shader stages.
    //------------------------------------------------------------------------

    // Type of buffer when used in a shader or render target.
    enum class BufferType
    {
        None = 0, // Not a buffer for a shader or render target.
        Typed, // Buffer where each element is a basic types: float, int or unsigned int
        Structured, // Buffer where each element is a struct. Format must be Unknown.
        Raw, // Buffer where each element is 4 bytes. Format must R32_TYPELESS.

        Total
    };

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
