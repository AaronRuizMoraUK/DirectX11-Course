#pragma once

#include <Graphics/Resource/ResourceFlags.h>

#include <variant>
#include <memory>

namespace DX
{
    class Texture;
    class Buffer;

    // This view only supports texture and buffer resources.
    struct RenderTargetViewDesc
    {
        using Resource = std::variant<std::shared_ptr<Texture>, std::shared_ptr<Buffer>>;
        Resource m_resource;

        // Format used by the view to read the resource.
        // Needs to be compatible with resource's underneath format,
        // explicitly set in textures and implicit in case of buffers.
        // For more details see TextureDesc.h and BufferDesc.h files.
        ResourceFormat m_viewFormat;

        // Only for textures
        uint32_t m_firstMip; // Index of the first mipmap level to use.

        // Only for textures 1D, 2D and Cube with array > 0
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arrayCount; // Number of arrays in the texture to use, starting from m_firstArray.

        // Only for 3D textures
        uint32_t m_firstDepth; // First depth level to use.
        uint32_t m_depthCount; // Number of depth levels to use, starting from m_firstDepth. -1 for all depths starting from m_firstDepth.

        // Only for Buffers.
        // Note it's the number of elements in the buffer, not bytes.
        uint32_t m_firstElement; // Index of the first element to use in the buffer.
        uint32_t m_numElements; // Number of elements in the buffer to use, starting from m_firstElement.
    };
} // namespace DX
