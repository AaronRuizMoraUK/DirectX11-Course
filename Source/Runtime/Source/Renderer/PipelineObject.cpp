#include <Renderer/PipelineObject.h>
#include <Renderer/Renderer.h>

#include <RHI/Device/Device.h>
#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>
#include <RHI/Shader/Shader.h>
#include <RHI/Pipeline/Pipeline.h>

namespace DX
{
    PipelineObject::PipelineObject(Renderer* renderer, const PipelineObjectDesc& desc)
    {
        PipelineDesc pipelineDesc = {};
        for (int i = 0; i < ShaderType_Count; ++i)
        {
            if (desc.m_shaderFilenames[i].empty())
            {
                continue;
            }
            const ShaderInfo shaderInfo{ static_cast<ShaderType>(i), desc.m_shaderFilenames[i], "main"};
            auto shaderByteCode = ShaderCompiler::Compile(shaderInfo);
            pipelineDesc.m_shaders[i] = renderer->GetDevice()->CreateShader({ shaderInfo, shaderByteCode });
        }
        pipelineDesc.m_inputLayout.m_inputElements = desc.m_inputElements;
        pipelineDesc.m_inputLayout.m_primitiveTopology = PrimitiveTopology::TriangleList;
        pipelineDesc.m_rasterizerState = {
            .m_faceFrontOrder = FaceFrontOrder::Clockwise,
            .m_faceCullMode = FaceCullMode::BackFace,
            .m_faceFillMode = FaceFillMode::Solid,
        };
        pipelineDesc.m_blendState.renderTargetBlends[0] = desc.m_blendState;
        pipelineDesc.m_depthStencilState = desc.m_depthStencilState;

        m_pipeline = renderer->GetDevice()->CreatePipeline(pipelineDesc);

        m_sceneResourceBindings = m_pipeline->CreateResourceBindingsObject();
        m_pipelineResourceBindings = m_pipeline->CreateResourceBindingsObject();
        m_materialResourceBindings = m_pipeline->CreateResourceBindingsObject();
        m_objectResourceBindings = m_pipeline->CreateResourceBindingsObject();
    }
} // namespace DX
