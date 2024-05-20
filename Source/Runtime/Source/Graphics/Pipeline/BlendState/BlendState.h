#pragma once

#include <Graphics/Pipeline/BlendState/BlendStateEnums.h>

#include <array>

namespace DX
{
    struct RenderTargetBlend
    {
        bool m_blendEnabled;
        Blend m_srcBlend;
        Blend m_destBlend;
        BlendOperation m_blendOp;
        Blend m_srcBlendAlpha;
        Blend m_destBlendAlpha;
        BlendOperation m_blendOpAlpha;

        ColorWriteMask m_colorWriteMask; // Bitwise operation of ColorWrite
    };

    struct BlendState
    {
        static constexpr int MaxRenderTargets = 8;
        using RenderTargetBlends = std::array<RenderTargetBlend, MaxRenderTargets>;

        RenderTargetBlends renderTargetBlends;
        bool m_independentBlendEnabled; // When false only first render target is used
        bool m_alphaToCoverageEnabled;
    };
} // namespace DX
