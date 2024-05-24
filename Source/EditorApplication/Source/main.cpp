#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>

// TODO: Try to remove and make it abstract in Runtime project
#include <RHI/Device/Device.h>
#include <RHI/Device/DeviceContext.h>
#include <RHI/Pipeline/Pipeline.h>
#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>

#include <Window/WindowManager.h>
#include <Assets/AssetManager.h>
#include <Math/Vector3.h>
#include <Math/Color.h>
#include <Log/Log.h>

#include <memory>
#include <chrono>

std::shared_ptr<DX::Pipeline> CreatePipeline()
{
    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.GetRenderer();

    const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType_Vertex, "Shaders/VertexShader.hlsl", "main" };
    const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType_Pixel, "Shaders/PixelShader.hlsl", "main" };
    auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
    auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);
    auto vertexShader = renderer->GetDevice()->CreateShader({vertexShaderInfo, vertexShaderByteCode});
    auto pixelShader = renderer->GetDevice()->CreateShader({ pixelShaderInfo, pixelShaderByteCode });

    DX::PipelineDesc pipelineDesc = {};
    pipelineDesc.m_shaders[DX::ShaderType_Vertex] = vertexShader;
    pipelineDesc.m_shaders[DX::ShaderType_Pixel] = pixelShader;
    pipelineDesc.m_inputLayout.m_inputElements =
    {
        DX::InputElement{ DX::InputSemantic::Position, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 0 },
        //InputElement{ DX::InputSemantic::Color, 0, DX::ResourceFormat::R32G32B32A32_FLOAT, 0, 12 },
        DX::InputElement{ DX::InputSemantic::TexCoord, 0, DX::ResourceFormat::R32G32_FLOAT, 0, 12 },
    };
    pipelineDesc.m_inputLayout.m_primitiveTopology = DX::PrimitiveTopology::TriangleList;
    pipelineDesc.m_rasterizerState = {
        .m_faceFrontOrder = DX::FaceFrontOrder::Clockwise,
        .m_faceCullMode = DX::FaceCullMode::BackFace,
        .m_faceFillMode = DX::FaceFillMode::Solid,
    };
    pipelineDesc.m_blendState.renderTargetBlends[0] = {
        .m_blendEnabled = false,
        .m_colorWriteMask = DX::ColorWrite_All
    };
    pipelineDesc.m_depthStencilState = {
        .m_depthEnabled = true,
        .m_depthTestFunc = DX::ComparisonFunction::Less,
        .m_depthWriteEnabled = true,
        .m_stencilEnabled = false
    };

    return renderer->GetDevice()->CreatePipeline(pipelineDesc);
}

int main()
{
    //const Math::Vector2Int windowSize{ 3440, 1440 };
    //const int refreshRate = 144;
    //const bool fullScreen = true;
    //const bool vSync = true;

    const Math::Vector2Int windowSize{ 1280, 720 };
    const int refreshRate = 144;
    const bool fullScreen = false;
    const bool vSync = true;

    // Asset Manager initialization
    DX::AssetManager::Get();

    // Window initialization
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle("DirectX11 Course", windowSize, refreshRate, fullScreen, vSync);
    if (!window)
    {
        return -1;
    }

    // Renderer initialization
    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.CreateRenderer(window);
    if (!renderer)
    {
        return -1;
    }

    auto pipeline = CreatePipeline();
    auto perViewResources = pipeline->CreateResourceBindingsObject();
    auto perObjectResources = pipeline->CreateResourceBindingsObject();

    // Camera
    auto camera = std::make_unique<DX::Camera>(Math::Vector3(2.0f, 1.0f, -2.0f), Math::Vector3(0.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Triangle>());
    objects.push_back(std::make_unique<DX::Cube>(Math::Vector3(1.0f)));
    objects.push_back(std::make_unique<DX::Mesh>("Models/DamagedHelmet/DamagedHelmet.gltf"));
    objects.push_back(std::make_unique<DX::Mesh>("Models/Jack/Jack.fbx"));
    objects[0]->SetTransform(Math::Vector3(0.0f, 1.0f, 0.0f));
    objects[1]->SetTransform(Math::Vector3(0.0f, 0.0f, 0.0f));
    objects[2]->SetTransform({ Math::Vector3(3.0f, 0.0f, 0.0f), Math::Quaternion::FromEulerAngles({ 0.0f, 3.14f, 0.0f }) });
    objects[3]->SetTransform({ Math::Vector3(1.5f, -1.0f, 0.0f), Math::Quaternion::FromEulerAngles({0.0f, 3.14f, 0.0f}), Math::Vector3(0.01f) });

    auto t0 = std::chrono::system_clock::now();

    while (window->IsOpen())
    {
        windowManager.PollEvents();

        // Calculate delta time
        const auto t1 = std::chrono::system_clock::now();
        const float deltaTime = std::chrono::duration<float>(t1 - t0).count();
        t0 = t1;
        //DX_LOG(Verbose, "Main", "Delta time: %f FPS: %0.1f", deltaTime, 1.0f / deltaTime);

        // ------
        // Update
        // ------
        camera->Update(deltaTime);
        //for (auto& object : objects)
        //{
        //    Math::Transform& transform = object->GetTransform();
        //    transform.m_rotation = transform.m_rotation * Math::Quaternion::FromEulerAngles(Math::Vector3(0.1f * deltaTime));
        //}

        // ------
        // Render
        // ------
        const Math::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
        renderer->Clear(clearColor, 1.0f, 0);

        renderer->BindFramebuffer();

        renderer->GetDevice()->GetImmediateContext().BindPipeline(*pipeline);

        camera->SetBuffers(*perViewResources);
        renderer->GetDevice()->GetImmediateContext().BindResources(*perViewResources);

        for (auto& object : objects)
        {
            object->SetBuffers(*perObjectResources);
            renderer->GetDevice()->GetImmediateContext().BindResources(*perObjectResources);

            renderer->Draw(object->GetIndexCount());
        }

        renderer->Present();
    }

    objects.clear();
    camera.reset();
    perObjectResources.reset();
    perViewResources.reset();
    pipeline.reset();
    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();
    DX::AssetManager::Destroy();

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
