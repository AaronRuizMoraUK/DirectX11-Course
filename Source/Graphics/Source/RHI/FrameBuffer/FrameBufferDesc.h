#pragma once

#include <memory>

namespace DX
{
    class Texture;

    struct FrameBufferDesc
    {
        // TODO: What about frame buffers with multiple color attachments?
        // TODO: What about frame buffers with buffer(s) attachments?

        std::shared_ptr<Texture> m_colorAttachment;
        std::shared_ptr<Texture> m_depthStencilAttachment;

        // When true a 2D depth stencil attachment will be created.
        // It'll have the same dimensions as the color attachment and format D24_UNORM_S8_UINT.
        bool m_createDepthStencilAttachment;
    };
} // namespace DX
