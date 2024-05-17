#pragma once

#include <Graphics/Resource/Buffer/BufferFlags.h>
#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    struct BufferDesc
    {
        // Type needs to be different than None when using the following bindings:
        // - BufferBind_ShaderResource
        // - BufferBind_ShaderRWResource
        // - BufferBind_RenderTarget
        BufferType m_bufferType;

        uint32_t m_sizeInBytes;
        ResourceUsage m_usage;
        BufferBindFlags m_bindFlags; // Bitwise operation of BufferBindFlag
        ResourceCPUAccess m_cpuAccess;

        // Only when using Structured buffer type. This is the size of the struct.
        uint32_t m_structSizeInBytes;

        void* m_initialData;
    };
} // namespace DX
