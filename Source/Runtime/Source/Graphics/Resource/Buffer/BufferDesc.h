#pragma once

#include <Graphics/Resource/Buffer/BufferEnums.h>
#include <Graphics/Resource/ResourceEnums.h>

namespace DX
{
    struct BufferDesc
    {
        // Type needs to be different than None when using any of the following bindings:
        // - BufferBind_ShaderResource
        // - BufferBind_ShaderRWResource
        // - BufferBind_RenderTarget
        BufferType m_bufferType;

        uint32_t m_elementSizeInBytes;
        uint32_t m_elementCount;
        ResourceUsage m_usage;
        BufferBindFlags m_bindFlags; // Bitwise operation of BufferBindFlag
        ResourceCPUAccess m_cpuAccess;

        void* m_initialData;
    };
} // namespace DX
