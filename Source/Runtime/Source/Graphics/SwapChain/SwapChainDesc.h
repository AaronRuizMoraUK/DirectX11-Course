#pragma once

#include <Graphics/Resource/ResourceFlags.h>

namespace DX
{
    class Window;

    struct SwapChainDesc
    {
        Window* m_window;
        int m_bufferCount;
        ResourceFormat m_bufferFormat;
    };
} // namespace DX
