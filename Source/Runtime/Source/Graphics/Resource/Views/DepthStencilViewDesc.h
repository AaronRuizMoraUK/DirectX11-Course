#pragma once

#include <memory>

namespace DX
{
    class Texture;

    // This view only supports texture resources.
    struct DepthStencilViewDesc
    {
        std::shared_ptr<Texture> m_texture;

        // Index of the first mipmap level to use.
        uint32_t m_firstMip;

        // Only for textures with array > 0
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arraySize; // Number of arrays in the textures to use starting from m_firstArray.
    };
} // namespace DX
