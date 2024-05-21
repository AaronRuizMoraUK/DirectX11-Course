#pragma once

#include <RHI/Resource/ResourceEnums.h>

#include <variant>
#include <memory>

namespace DX
{
    class Texture;
    class Buffer;

    // This view only supports texture and buffer resources.
    struct ShaderResourceViewDesc
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
        int m_mipCount; // Number of mipmap levels to use, starting from m_firstMip. -1 for all mipmaps starting from m_firstMip.

        // Only for textures 1D, 2D and Cube with array > 1
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arrayCount; // Number of arrays in the texture to use, starting from m_firstArray.

        // Only for Buffers.
        // Note it's the number of elements in the buffer, not bytes.
        uint32_t m_firstElement; // Index of the first element to use in the buffer.
        uint32_t m_elementCount; // Number of elements in the buffer to use, starting from m_firstElement.
    };
} // namespace DX
