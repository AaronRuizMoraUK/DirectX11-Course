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
} // namespace DX
