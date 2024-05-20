#pragma once

#include <Graphics/Resource/ResourceEnums.h>

namespace DX
{
    enum class StencilOp
    {
        Unknown = 0,

        Keep,
        Zero,
        Replace,
        IncrementClamp,
        DecrementClamp,
        Invert,
        Increment,
        Decrement,

        Count
    };

    struct StencilBehaviour
    {
        StencilOp m_stencilFailOp;
        StencilOp m_stencilDepthFailOp;
        StencilOp m_stencilPassOp;
        ComparisonFunction m_stencilComparisonFunction;
    };
} // namespace DX
