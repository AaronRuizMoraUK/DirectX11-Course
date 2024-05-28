#include <Renderer/Scene.h>
#include <Renderer/Renderer.h>
#include <Renderer/PipelineObject.h>
#include <Renderer/Object.h>
#include <Window/Window.h>
#include <Camera/Camera.h>

#include <RHI/Device/Device.h>
#include <RHI/Device/DeviceContext.h>
#include <RHI/CommandList/CommandList.h>
#include <RHI/Pipeline/PipelineResourceBindings.h>
#include <RHI/Resource/Buffer/Buffer.h>

#include <Math/Vector2.h>

namespace DX
{
    Scene::Scene(Renderer* renderer)
        : m_renderer(renderer)
    {
        // Create pipeline object
        {
            PipelineObjectDesc pipelineObjectDesc = {};
            pipelineObjectDesc.m_shaderFilenames[ShaderType_Vertex] = "Shaders/VertexShader.hlsl";
            pipelineObjectDesc.m_shaderFilenames[ShaderType_Pixel] = "Shaders/PixelShader.hlsl";
            pipelineObjectDesc.m_inputElements = {
                DX::InputElement{ DX::InputSemantic::Position, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 0 },
                DX::InputElement{ DX::InputSemantic::Normal, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 12 },
                DX::InputElement{ DX::InputSemantic::Tangent, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 24 },
                DX::InputElement{ DX::InputSemantic::Binormal, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 36 },
                DX::InputElement{ DX::InputSemantic::TexCoord, 0, DX::ResourceFormat::R32G32_FLOAT, 0, 48 },
            };
            pipelineObjectDesc.m_blendState = {
                .m_blendEnabled = false,
                .m_colorWriteMask = DX::ColorWrite_All
            };
            pipelineObjectDesc.m_depthStencilState = {
                .m_depthEnabled = true,
                .m_depthTestFunc = DX::ComparisonFunction::Less,
                .m_depthWriteEnabled = true,
                .m_stencilEnabled = false
            };

            m_pipelineObject = std::make_unique<PipelineObject>(renderer, pipelineObjectDesc);
        }

        // Per Scene Resources
        {
            const ViewProjBuffer viewProjBuffer;

            BufferDesc constantBufferDesc = {};
            constantBufferDesc.m_elementSizeInBytes = sizeof(ViewProjBuffer);
            constantBufferDesc.m_elementCount = 1;
            constantBufferDesc.m_usage = ResourceUsage::Dynamic;
            constantBufferDesc.m_bindFlags = BufferBind_ConstantBuffer;
            constantBufferDesc.m_cpuAccess = ResourceCPUAccess::Write;
            constantBufferDesc.m_bufferSubType = BufferSubType::None;
            constantBufferDesc.m_initialData = &viewProjBuffer;

            m_viewProjMatrixConstantBuffer = renderer->GetDevice()->CreateBuffer(constantBufferDesc);
        }

        // Per Object Resources
        {
            const WorldBuffer worldBuffer;

            BufferDesc constantBufferDesc = {};
            constantBufferDesc.m_elementSizeInBytes = sizeof(WorldBuffer);
            constantBufferDesc.m_elementCount = 1;
            constantBufferDesc.m_usage = ResourceUsage::Dynamic;
            constantBufferDesc.m_bindFlags = BufferBind_ConstantBuffer;
            constantBufferDesc.m_cpuAccess = ResourceCPUAccess::Write;
            constantBufferDesc.m_bufferSubType = BufferSubType::None;
            constantBufferDesc.m_initialData = &worldBuffer;

            m_worldMatrixConstantBuffer = renderer->GetDevice()->CreateBuffer(constantBufferDesc);
        }

        // Command List
        m_commandList = renderer->GetDevice()->CreateCommandList();
    }

    Scene::~Scene() = default;

    void Scene::SetCamera(Camera* camera)
    {
        m_camera = camera;
    }

    void Scene::AddObject(Object* object)
    {
        m_objects.insert(object);
    }

    void Scene::RemoveObject(Object* object)
    {
        m_objects.erase(object);
    }

    void Scene::Render()
    {
        // Update constant buffer with the latest view and projection matrices.
        {
            const ViewProjBuffer viewProjBuffer = { 
                m_camera->GetViewMatrix(), 
                m_camera->GetProjectionMatrix(),
                Math::Vector4Packed{Math::Vector4{m_camera->GetTransform().m_position, 1.0f}}
            };

            m_renderer->GetDevice()->GetImmediateContext().UpdateDynamicBuffer(*m_viewProjMatrixConstantBuffer, &viewProjBuffer, sizeof(ViewProjBuffer));
        }

        // Draw all objects that use the same pipeline asynchronously
        drawObjects = std::async(std::launch::async, [&]()
            {
                // Bind frame buffer and viewport
                m_commandList->GetDeferredContext().BindFrameBuffer(*m_renderer->GetFrameBuffer());
                m_commandList->GetDeferredContext().BindViewports({ 
                    Math::Rectangle{{0.0f, 0.0f}, 
                    Math::Vector2{m_renderer->GetWindow()->GetSize()}}
                });

                // Bind pipeline
                m_commandList->GetDeferredContext().BindPipeline(*m_pipelineObject->GetPipeline());

                // Bind per Scene resources
                {
                    m_pipelineObject->GetSceneResourceBindings()->SetConstantBuffer(ShaderType_Vertex, 0, m_viewProjMatrixConstantBuffer);

                    m_commandList->GetDeferredContext().BindResources(*m_pipelineObject->GetSceneResourceBindings());
                }

                for (auto* object : m_objects)
                {
                    // Bind per Material resources
                    {
                        m_pipelineObject->GetMaterialResourceBindings()->SetShaderResourceView(ShaderType_Pixel, 0, object->GetTextureView());
                        m_pipelineObject->GetMaterialResourceBindings()->SetShaderResourceView(ShaderType_Pixel, 1, object->GetNormalTextureView());
                        m_pipelineObject->GetMaterialResourceBindings()->SetSampler(ShaderType_Pixel, 0, object->GetSampler());
                        m_pipelineObject->GetMaterialResourceBindings()->SetSampler(ShaderType_Pixel, 1, object->GetNormalSampler());

                        m_commandList->GetDeferredContext().BindResources(*m_pipelineObject->GetMaterialResourceBindings());
                    }

                    // Bind per Object resources
                    {
                        // Update constant buffer with the object's world matrix.
                        // If there were multiple command lists, then it would require multiple m_worldMatrixConstantBuffer.
                        {
                            const WorldBuffer worldBuffer = { 
                                object->GetTransform().ToMatrix(), 
                                object->GetTransform().ToMatrix().Inverse().Transpose()
                            };

                            m_commandList->GetDeferredContext().UpdateDynamicBuffer(*m_worldMatrixConstantBuffer, &worldBuffer, sizeof(worldBuffer));
                        }
                        m_pipelineObject->GetObjectResourceBindings()->SetConstantBuffer(ShaderType_Vertex, 1, m_worldMatrixConstantBuffer);
                        m_pipelineObject->GetObjectResourceBindings()->SetConstantBuffer(ShaderType_Pixel, 1, m_worldMatrixConstantBuffer);

                        m_commandList->GetDeferredContext().BindResources(*m_pipelineObject->GetObjectResourceBindings());
                    }

                    // Bind Vertex and Index Buffers
                    m_commandList->GetDeferredContext().BindVertexBuffers({ object->GetVertexBuffer().get() });
                    m_commandList->GetDeferredContext().BindIndexBuffer(*object->GetIndexBuffer());

                    // Draw
                    m_commandList->GetDeferredContext().DrawIndexed(object->GetIndexCount());
                }

                m_commandList->FinishCommandList();
            });
    }

    void Scene::WaitAndExecute()
    {
        if (drawObjects.valid())
        {
            drawObjects.wait();

            m_renderer->GetDevice()->ExecuteCommandLists({ m_commandList.get() });
        }
    }
} // namespace DX
