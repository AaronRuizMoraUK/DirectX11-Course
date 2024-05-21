#pragma once

#include <RHI/Shader/Shader.h>
#include <RHI/Pipeline/InputLayout/InputLayout.h>
#include <RHI/Pipeline/RasterizerState/RasterizerState.h>
#include <RHI/Pipeline/BlendState/BlendState.h>
#include <RHI/Pipeline/DepthStencilState/DepthStencilState.h>

#include <array>
#include <memory>

namespace DX
{
    using PipelineShaders = std::array<std::shared_ptr<Shader>, ShaderType_Count>;

    struct PipelineDesc
    {
        PipelineShaders m_shaders;
        InputLayout m_inputLayout;
        RasterizerState m_rasterizerState;
        BlendState m_blendState;
        DepthStencilState m_depthStencilState;
    };
} // namespace DX
