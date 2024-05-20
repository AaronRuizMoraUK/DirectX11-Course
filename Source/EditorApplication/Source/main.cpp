#define SIMPLE_RENDERER 0

#if SIMPLE_RENDERER
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>
#else
#include <Graphics/Device/DeviceManager.h>
#include <Graphics/Device/DeviceContext.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/CommandList/CommandList.h>
#include <Graphics/Pipeline/Pipeline.h>
#include <Graphics/Shader/ShaderCompiler/ShaderCompiler.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#endif

#include <Window/WindowManager.h>
#include <Math/Vector3.h>
#include <Math/Color.h>
#include <Log/Log.h>
#include <Tests/UnitTests.h>

#include <memory>
#include <thread>
#include <future>

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

    // Window initialization
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle("DirectX11 Course", windowSize, refreshRate, fullScreen, vSync);
    if (!window)
    {
        return -1;
    }

#if SIMPLE_RENDERER

    // Renderer initialization
    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.CreateRenderer(window);
    if (!renderer)
    {
        return -1;
    }

    // Camera
    auto camera = std::make_unique<DX::Camera>(Math::Vector3(2.0f, 1.0f, -2.0f), Math::Vector3(0.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Triangle>());
    objects.push_back(std::make_unique<DX::Cube>(Math::Vector3(1.0f)));
    objects[0]->SetTransform(Math::Vector3(0.0f, 1.0f, 0.0f));

    while (window->IsOpen())
    {
        windowManager.PollEvents();

        // ------
        // Update
        // ------
        constexpr float deltaTime = 1.0f / static_cast<float>(refreshRate); // TODO: Calculate delta time
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
        renderer->ClearColor(clearColor);

        renderer->SetPipeline();

        camera->SetBuffers();

        for (auto& object : objects)
        {
            object->SetBuffers();

            renderer->Draw(object->GetIndexCount());
        }

        renderer->Present();
    }

    objects.clear();
    camera.reset();
    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();

#else

    // Graphics device initialization
    DX::DeviceManager& deviceManager = DX::DeviceManager::Get();
    DX::Device* device = deviceManager.CreateDevice();
    if (!device)
    {
        return -1;
    }

    // Tests creating all device objects
    UnitTest::TestsDeviceObjects();

    struct Vertex
    {
        Math::Vector3Packed position;
        Math::ColorPacked color;
    };

    // Clockwise order (CW) - LeftHand

    const Vertex triangleVertices[]  =
    {
        { Math::Vector3Packed({-0.5f, -0.5f, 0.0f}), Math::ColorPacked(Math::Colors::Red) },
        { Math::Vector3Packed({0.0f,   0.5f, 0.0f}), Math::ColorPacked(Math::Colors::Green) },
        { Math::Vector3Packed({0.5f,  -0.5f, 0.0f}), Math::ColorPacked(Math::Colors::Blue) }
    };

    const uint32_t triangleIndices[] =
    {
        0, 1, 2
    };

    {
        auto swapChain = device->CreateSwapChain({ window, 1, DX::ResourceFormat::R8G8B8A8_UNORM });
        auto frameBuffer = device->CreateFrameBuffer({ swapChain->GetBackBufferTexture(), nullptr, true /*Create Depth Stencil*/});

        const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType_Vertex, "Shaders/VertexShader.hlsl", "mainColor" };
        const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType_Pixel, "Shaders/PixelShader.hlsl", "mainColor" };
        auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);
        auto vertexShader = device->CreateShader({ vertexShaderInfo, vertexShaderByteCode });
        auto pixelShader = device->CreateShader({ pixelShaderInfo, pixelShaderByteCode });

        DX::PipelineDesc pipelineDesc = {};
        pipelineDesc.m_shaders[DX::ShaderType_Vertex] = vertexShader;
        pipelineDesc.m_shaders[DX::ShaderType_Pixel] = pixelShader;
        pipelineDesc.m_inputLayout.m_inputElements =
        {
            DX::InputElement{ DX::InputSemantic::Position, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 0 },
            DX::InputElement{ DX::InputSemantic::Color, 0, DX::ResourceFormat::R32G32B32A32_FLOAT, 0, 12 },
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
        auto pipelineTriangle = device->CreatePipeline(pipelineDesc);

        auto cmdListTriangle = device->CreateCommandList();

        DX::BufferDesc vertexBufferDesc = {};
        vertexBufferDesc.m_elementSizeInBytes = sizeof(Vertex);
        vertexBufferDesc.m_elementCount = 3;
        vertexBufferDesc.m_usage = DX::ResourceUsage::Immutable;
        vertexBufferDesc.m_bindFlags = DX::BufferBind_VertexBuffer;
        vertexBufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        vertexBufferDesc.m_initialData = triangleVertices;
        auto vertexBuffer = device->CreateBuffer(vertexBufferDesc);

        DX::BufferDesc indexBufferDesc = {};
        indexBufferDesc.m_elementSizeInBytes = sizeof(uint32_t);
        indexBufferDesc.m_elementCount = 3;
        indexBufferDesc.m_usage = DX::ResourceUsage::Immutable;
        indexBufferDesc.m_bindFlags = DX::BufferBind_IndexBuffer;
        indexBufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        indexBufferDesc.m_initialData = triangleIndices;
        auto indexBuffer = device->CreateBuffer(indexBufferDesc);

        auto t0 = std::chrono::system_clock::now();

        while (window->IsOpen())
        {
            windowManager.PollEvents();

            // ------
            // Update
            // ------
            const auto t1 = std::chrono::system_clock::now();
            const float deltaTime = std::chrono::duration<float>(t1 - t0).count();
            t0 = t1;
            //DX_LOG(Verbose, "Main", "Delta time: %f FPS: %0.1f", deltaTime, 1.0f / deltaTime);

            // ------
            // Render
            // ------
            const Math::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
            device->GetImmediateContext().ClearFrameBuffer(*frameBuffer, clearColor, 1.0f, 0);

            std::future drawTriangle = std::async(std::launch::async, [&]()
                {
                    cmdListTriangle->GetDeferredContext().BindFrameBuffer(*frameBuffer);
                    cmdListTriangle->GetDeferredContext().BindViewports({ Math::Rectangle{{0.0f, 0.0f}, Math::Vector2{window->GetSize()}} });

                    cmdListTriangle->GetDeferredContext().BindPipeline(*pipelineTriangle);

                    cmdListTriangle->GetDeferredContext().BindVertexBuffers({ vertexBuffer.get() });
                    cmdListTriangle->GetDeferredContext().BindIndexBuffer(*indexBuffer);

                    cmdListTriangle->GetDeferredContext().DrawIndexed(indexBuffer->GetBufferDesc().m_elementCount);

                    cmdListTriangle->FinishCommandList();
                });

            drawTriangle.wait();

            device->ExecuteCommandLists({ cmdListTriangle.get() });

            swapChain->Present();
        }
    }

    DX::DeviceManager::Destroy();
    DX::WindowManager::Destroy();

#endif // SIMPLE_RENDERER

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
