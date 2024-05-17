#pragma once

#include <Graphics/Resource/ResourceFlags.h>

#include <memory>

namespace DX
{
    class Texture;

    // This view only supports texture resources.
    struct DepthStencilViewDesc
    {
        std::shared_ptr<Texture> m_texture;

        // Format used by the view to read the resource.
        // Needs to be compatible with texture's format.
        // For more details see TextureDesc.h file.
        ResourceFormat m_viewFormat;

        // Index of the first mipmap level to use.
        uint32_t m_firstMip;

        // Only for textures 1D, 2D and Cube with array > 1
        uint32_t m_firstArray; // Index of the first texture to use in an array of textures.
        uint32_t m_arrayCount; // Number of arrays in the textures to use, starting from m_firstArray.
    };
} // namespace DX
