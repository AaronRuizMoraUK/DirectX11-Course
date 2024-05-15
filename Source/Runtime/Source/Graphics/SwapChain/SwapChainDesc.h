#pragma once

#include <Graphics/Texture/TextureFormat.h>

namespace DX
{
    class Window;

    struct SwapChainDesc
    {
        Window* m_window;
        int m_bufferCount;
        TextureFormat m_bufferFormat;
    };
} // namespace DX
