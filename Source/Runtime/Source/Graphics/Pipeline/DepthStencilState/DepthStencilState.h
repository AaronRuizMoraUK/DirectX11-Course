#pragma once

#include <Graphics/Pipeline/DepthStencilState/DepthStencilStateEnums.h>

namespace DX
{
    struct DepthStencilState
    {
        bool m_depthEnabled;
        ComparisonFunction m_depthTestFunc;
        bool m_depthWriteEnabled;

        bool m_stencilEnabled;
        uint8_t m_stencilReadMask;
        uint8_t m_stencilWriteMask;
        StencilBehaviour m_frontFaceStencilBehaviour;
        StencilBehaviour m_backFaceStencilBehaviour;
    };
} // namespace DX
