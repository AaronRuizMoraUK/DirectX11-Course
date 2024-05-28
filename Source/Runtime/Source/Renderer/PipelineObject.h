#pragma once

#include <RHI/Shader/ShaderEnums.h>
#include <RHI/Pipeline/InputLayout/InputLayout.h>
#include <RHI/Pipeline/BlendState/BlendState.h>
#include <RHI/Pipeline/DepthStencilState/DepthStencilState.h>

#include <memory>
#include <vector>

namespace DX
{
    class Renderer;
    class Pipeline;
    class PipelineResourceBindings;

    struct PipelineObjectDesc
    {
        using ShaderFilenames = std::array<std::string, ShaderType_Count>;

        ShaderFilenames m_shaderFilenames;
        std::vector<InputElement> m_inputElements;
        RenderTargetBlend m_blendState;
        DepthStencilState m_depthStencilState;
    };

    class PipelineObject
    {
    public:
        PipelineObject(Renderer* renderer, const PipelineObjectDesc& desc);
        ~PipelineObject() = default;

        Pipeline* GetPipeline() { return m_pipeline.get(); }

        PipelineResourceBindings* GetSceneResourceBindings() { return m_sceneResourceBindings.get(); }
        PipelineResourceBindings* GetPipelineResourceBindings() { return m_pipelineResourceBindings.get(); }
        PipelineResourceBindings* GetMaterialResourceBindings() { return m_materialResourceBindings.get(); }
        PipelineResourceBindings* GetObjectResourceBindings() { return m_objectResourceBindings.get(); }

    private:
        std::shared_ptr<Pipeline> m_pipeline;
        std::shared_ptr<PipelineResourceBindings> m_sceneResourceBindings;
        std::shared_ptr<PipelineResourceBindings> m_pipelineResourceBindings;
        std::shared_ptr<PipelineResourceBindings> m_materialResourceBindings;
        std::shared_ptr<PipelineResourceBindings> m_objectResourceBindings;
    };
} // namespace DX
