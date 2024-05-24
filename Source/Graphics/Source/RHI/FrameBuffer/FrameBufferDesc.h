#pragma once

#include <RHI/Resource/ResourceEnums.h>

#include <memory>
#include <vector>
#include <variant>

namespace DX
{
    class Texture;
    class Buffer;

    struct FrameBufferDesc
    {
        struct TextureAttachment
        {
            std::shared_ptr<Texture> m_texture;
            ResourceFormat m_viewFormat;
        };

        struct BufferAttachment
        {
            std::shared_ptr<Buffer> m_buffer;
            ResourceFormat m_viewFormat;
        };

        using TextureAttachments = std::vector<TextureAttachment>;
        using BufferAttachments = std::vector<BufferAttachment>;

        using RenderTargetAttachments = std::variant<TextureAttachments, BufferAttachments>;

        RenderTargetAttachments m_renderTargetAttachments;
        TextureAttachment m_depthStencilAttachment;

        // When true, the 2D depth stencil attachment will be created. Only use when render target attachments are Textures.
        // It'll have the same dimensions as the first texture render target attachment and format D24_UNORM_S8_UINT.
        bool m_createDepthStencilAttachment;
    };
} // namespace DX
