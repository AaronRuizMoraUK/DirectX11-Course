#pragma once

#include <Graphics/Shader/Shader.h>
#include <Graphics/Pipeline/InputLayout/InputLayout.h>
#include <Graphics/Pipeline/RasterizerState/RasterizerState.h>
#include <Graphics/Pipeline/BlendState/BlendState.h>
#include <Graphics/Pipeline/DepthStencilState/DepthStencilState.h>

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
