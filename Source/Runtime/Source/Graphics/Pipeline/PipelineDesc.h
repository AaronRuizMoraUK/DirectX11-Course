#pragma once

#include <Graphics/Pipeline/InputLayout/InputLayout.h>
#include <Graphics/Pipeline/RasterizerState/RasterizerState.h>
#include <Graphics/Pipeline/BlendState/BlendState.h>
#include <Graphics/Pipeline/DepthStencilState/DepthStencilState.h>

namespace DX
{
    struct PipelineDesc
    {
        InputLayout m_inputLayout;
        RasterizerState m_rasterizerState;
        BlendState m_blendState;
        DepthStencilState m_depthStencilState;
    };
} // namespace DX
