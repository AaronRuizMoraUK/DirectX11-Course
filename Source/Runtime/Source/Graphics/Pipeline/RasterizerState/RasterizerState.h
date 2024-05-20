#pragma once

#include <Graphics/Pipeline/RasterizerState/RasterizerStateEnums.h>

namespace DX
{
    struct RasterizerState
    {
        FaceFrontOrder m_faceFrontOrder;
        FaceCullMode m_faceCullMode;
        FaceFillMode m_faceFillMode;

        // Depth Bias
        int m_depthBias;
        float m_depthBiasClamp;
        float m_slopeScaledDepthBias;

        bool m_depthClipDisabled; // Asking if disabled so when doing "RasterizerState rs = {};" it's enabled by default
        bool m_scissorEnabled;
        bool m_multisampleEnabled;
        bool m_antialiasedLineEnabled;
    };
} // namespace DX
