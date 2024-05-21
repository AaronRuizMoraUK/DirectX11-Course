#pragma once

#include <RHI/Resource/ResourceEnums.h>
#include <Math/Vector2.h>

namespace DX
{
    struct SwapChainDesc
    {
        Math::Vector2Int m_size;
        uint32_t m_refreshRate;
        bool m_fullScreen;
        bool m_vSyncEnabled;

        uint32_t m_bufferCount;
        ResourceFormat m_bufferFormat;

        void* m_nativeWindowHandler;
    };
} // namespace DX
