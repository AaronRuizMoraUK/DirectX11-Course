#pragma once

#include <Graphics/Resource/ResourceFlags.h>

#include <memory>

namespace DX
{
    class SwapChain;
    class Texture;

    struct FrameBufferDesc
    {
        // If swap chain is set, it will be used to create the frame buffer's attachments.
        // 
        // TODO: Can SwapChain internally use Texture classes so this can be filled directly?
        SwapChain* m_swapChain;

        std::shared_ptr<Texture> m_colorAttachment;
        std::shared_ptr<Texture> m_depthStencilAttachment;
    };
} // namespace DX
