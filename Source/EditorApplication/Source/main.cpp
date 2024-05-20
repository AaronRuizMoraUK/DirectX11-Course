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
#endif

#include <Window/WindowManager.h>
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
    DX::Device* device = deviceManager.CreateDevice({});
    if (!device)
    {
        return -1;
    }

    // Tests creating all device objects
    {
        auto swapChain = device->CreateSwapChain({ window, 1, DX::ResourceFormat::R8G8B8A8_UNORM });
        auto frameBuffer = device->CreateFrameBuffer({ swapChain->GetBackBufferTexture(), nullptr, true /*Create Depth Stencil*/});

        UnitTest::TestsDeviceObjects();

        auto cmdListTriangle = device->CreateCommandList();
        auto cmdListCube = device->CreateCommandList();

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
            device->GetImmediateContext().ClearFrameBuffer(*frameBuffer, clearColor);

            device->GetImmediateContext().BindFrameBuffer(*frameBuffer);
            device->GetImmediateContext().BindViewports({ Math::Rectangle{{0.0f, 0.0f}, Math::Vector2{window->GetSize()}} });

            std::future drawTriangle = std::async(std::launch::async, [cmdListTriangle]()
                {
                    //cmdListTriangle->GetDeferredContext().BindPipeline();

                    //cmdListTriangle->GetDeferredContext().BindPVertexBuffer();
                    //cmdListTriangle->GetDeferredContext().BindPIndexBuffer();
                    //cmdListTriangle->GetDeferredContext().BindPResource();

                    //cmdListTriangle->GetDeferredContext().DrawIndexed();

                    cmdListTriangle->FinishCommandList();
                });

            std::future drawCube = std::async(std::launch::async, [cmdListCube]()
                {
                    //cmdListCube->GetDeferredContext().BindPipeline();

                    //cmdListCube->GetDeferredContext().BindPVertexBuffer();
                    //cmdListCube->GetDeferredContext().BindPIndexBuffer();
                    //cmdListCube->GetDeferredContext().BindPResource();

                    //cmdListCube->DrawIndexed();

                    cmdListCube->FinishCommandList();
                });

            drawTriangle.wait();
            drawCube.wait();

            device->ExecuteCommandLists({ cmdListTriangle.get(), cmdListCube.get() });

            swapChain->Present();
        }
    }

    DX::DeviceManager::Destroy();
    DX::WindowManager::Destroy();

#endif // SIMPLE_RENDERER

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
