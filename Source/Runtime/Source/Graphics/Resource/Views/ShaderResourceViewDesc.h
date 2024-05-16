#pragma once

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

        // Only for textures
        uint32_t m_firstMip; // Index of the first mipmap level to use.
        uint32_t m_mipSize; // Number of mipmap levels to use starting from m_firstMip. -1 for all mipmaps starting from m_firstMip.

        // Only for textures 1D, 2D and Cube with array > 0
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arraySize; // Number of arrays in the textures to use starting from m_firstArray.

        // Only for Buffers
        uint32_t m_firstElement; // Number of bytes between the beginning of the buffer and the first element to access.
        uint32_t m_numElements; // The total number of elements in the view.
    };
} // namespace DX
