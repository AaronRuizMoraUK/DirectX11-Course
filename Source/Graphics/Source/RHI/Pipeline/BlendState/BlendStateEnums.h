#pragma once

namespace DX
{
    enum class Blend
    {
        Unknown = 0,

        Zero,
        One,
        SrcColor,
        InvSrcColor,
        SrcAlpha,
        InvSrcAlpha,
        DestAlpha ,
        InvDestAlpha,
        DestColor,
        InvDestColor,
        SrcAlphaSat,
        BlendFactor,
        InvBlendFactor,
        Src1Color,
        InvSrc1Color,
        Src1Alpha,
        InvSrc1Alpha,

        Count
    };

    enum class BlendOperation
    {
        Unknown = 0,

        Add,
        Subtract,
        RevSubtract,
        Min,
        Max,

        Count
    };

    // Bitwise operations on ResourceBindFlag are allowed.
    enum ColorWrite
    {
        ColorWrite_Red = 1 << 0,
        ColorWrite_Green = 1 << 1,
        ColorWrite_Blue = 1 << 2,
        ColorWrite_Alpha = 1 << 3,

        ColorWrite_All = ColorWrite_Red | ColorWrite_Green | ColorWrite_Blue | ColorWrite_Alpha,
    };

    using ColorWriteMask = uint8_t;

} // namespace DX
