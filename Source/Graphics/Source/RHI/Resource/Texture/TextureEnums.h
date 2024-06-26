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

    const char* TextureTypeStr(TextureType textureType);

    // Used by shader resource layout
    enum TextureSubTypeFlag
    {
        TextureSubType_Array = 1 << 0,
        TextureSubType_Multisample = 1 << 1
    };

    using TextureSubTypeFlags = uint32_t;

    enum TextureBindFlag
    {
        TextureBind_ShaderResource = 1 << 0,
        TextureBind_ShaderRWResource = 1 << 1,
        TextureBind_RenderTarget = 1 << 2,
        TextureBind_DepthStencil = 1 << 3,
    };

    using TextureBindFlags = uint32_t;

} // namespace DX
