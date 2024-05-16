#pragma once

#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    //------------------------------------------------------------------------
    // Buffer Format
    // 
    // Buffer format is implicit and not set inside the description.
    // The format of the buffer is set later depending on what's used for (bind):
    // 
    // - As VertexBuffer: format is specified with an InputLayout, which is set with DeviceContext->IASetInputLayout() call.
    // - As IndexBuffer: format is specified in DeviceContext->IASetIndexBuffer() call.
    // - As ConstantBuffer: format is expected to match the structure of the constant buffer in the shader.
    // - As ShaderResource, ShaderRWResource or RenderTarget: format is specified in their respective view description.
    // - As StreamOutput: format is always 32-bit values coming from the output declarations of the shader stages.
    //------------------------------------------------------------------------

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

    // Type of buffer when used in a shader or render target.
    enum class BufferType
    {
        Unknown,

        Typed, // Buffer where each element is a basic types: float, int or unsigned int
        Structured, // Buffer where each element is a struct
        Raw, // Buffer where each element is a byte

        Total
    };

    struct BufferDesc
    {
        uint32_t m_sizeInBytes;
        ResourceUsage m_usage;
        BufferBindFlag m_bindFlag;
        ResourceCPUAccess m_cpuAccess;

        // Only relevant when using one of the following binding options:
        // - BufferBind_ShaderResource
        // - BufferBind_ShaderRWResource
        // - BufferBind_RenderTarget
        BufferType m_bufferType;

        // Only when using Structured buffer type. This is the size of the struct.
        uint32_t m_structSizeInBytes;

        void* m_initialData;
    };
} // namespace DX
