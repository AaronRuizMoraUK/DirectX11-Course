#pragma once

#include <variant>
#include <memory>

namespace DX
{
    class Texture;
    class Buffer;

    // This view only supports texture and buffer resources.
    struct ShaderRWResourceViewDesc
    {
        using Resource = std::variant<std::shared_ptr<Texture>, std::shared_ptr<Buffer>>;
        Resource m_resource;

        // Only for textures
        uint32_t m_firstMip; // Index of the first mipmap level to use.

        // Only for textures 1D, 2D and Cube with array > 0
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arraySize; // Number of arrays in the textures to use starting from m_firstArray.

        // Only for 3D textures
        uint32_t m_firstDepth; // First depth level to use.
        uint32_t m_depthSize; // Number of depth levels to use starting from m_firstDepth. -1 for all depths starting from m_firstDepth.

        // Only for Buffers
        uint32_t m_firstElement; // Number of bytes between the beginning of the buffer and the first element to access.
        uint32_t m_numElements; // The total number of elements in the view.
    };
} // namespace DX
