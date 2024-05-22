#pragma once

#include <cstdint>

namespace DX
{
    enum class TextureType
    {
        Unknown = 0,

        Texture1D,
        Texture2D,
        TextureCube, // A two dimensional texture array with 6 sub-resources
        Texture3D,

        Count
    };

    // Used by shader resource layout
    enum TextureSubTypeFlag
    {
        Array = 1 << 0,
        Multisample = 1 << 1
    };

    using TextureSubTypeFlags = uint32_t;

    //------------------------------------------------------------------------
    // Texture Format
    // 
    // Texture format is set explicitly inside the resource.
    // All its usages are views where the format must is specified too.
    // The texture format and the view format must be compatible.
    //------------------------------------------------------------------------

    enum TextureBindFlag
    {
        TextureBind_ShaderResource = 1 << 0,
        TextureBind_ShaderRWResource = 1 << 1,
        TextureBind_RenderTarget = 1 << 2,
        TextureBind_DepthStencil = 1 << 3,
    };

    using TextureBindFlags = uint32_t;

} // namespace DX
