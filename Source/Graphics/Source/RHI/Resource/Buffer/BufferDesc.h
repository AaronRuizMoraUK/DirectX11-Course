#pragma once

#include <RHI/Resource/Buffer/BufferEnums.h>
#include <RHI/Resource/ResourceEnums.h>

namespace DX
{
    struct BufferDesc
    {
        uint32_t m_elementSizeInBytes;
        uint32_t m_elementCount;
        ResourceUsage m_usage;
        BufferBindFlags m_bindFlags; // Bitwise operation of BufferBindFlag
        ResourceCPUAccess m_cpuAccess;

        // Subtype needs to be different than None when using any of the following bindings:
        // - BufferBind_ShaderResource
        // - BufferBind_ShaderRWResource
        // - BufferBind_RenderTarget
        BufferSubType m_bufferSubType;

        const void* m_initialData;
    };
} // namespace DX
