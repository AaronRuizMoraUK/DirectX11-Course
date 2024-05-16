#pragma once

#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    // Allows bitwise operations on ResourceBindFlag
    // Buffer format is not set inside the resource, the location differs depending on what's used for (bind):
    // - VertexBuffer: format is specified with an InputLayout, which is set with DeviceContext->IASetInputLayout() call
    // - IndexBuffer: format is specified in DeviceContext->IASetIndexBuffer() call
    // - ConstantBuffer: ??
    // - ShaderResource, ShaderRWResource, RenderTarget: format is specified in the views.
    // - StreamOutput: ??
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

    struct BufferDesc
    {
        ResourceFormat m_format; // TODO: Necessary?
        ResourceUsage m_usage;
        BufferBindFlag m_bindFlag;
        ResourceCPUAccess m_cpuAccess;

        void* m_data;
    };
} // namespace DX
