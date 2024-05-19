#define SIMPLE_RENDERER 0

#if SIMPLE_RENDERER
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>
#else
#include <Graphics/Device/DeviceManager.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/CommandList/CommandList.h>
#endif

#include <Window/WindowManager.h>
#include <Log/Log.h>
#include <Math/Color.h>
#include <Tests/UnitTests.h>

#include <memory>
#include <thread>
#include <future>

int main()
{
    //const mathfu::Vector2Int windowSize{ 3440, 1440 };
    //const int refreshRate = 144;
    //const bool fullScreen = true;
    //const bool vSync = true;

    const mathfu::Vector2Int windowSize{ 1280, 720 };
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
    auto camera = std::make_unique<DX::Camera>(mathfu::Vector3(2.0f, 1.0f, -2.0f), mathfu::Vector3(0.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Triangle>());
    objects.push_back(std::make_unique<DX::Cube>(mathfu::Vector3(1.0f)));
    objects[0]->SetTransform(mathfu::Vector3(0.0f, 1.0f, 0.0f));

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
        //    mathfu::Transform& transform = object->GetTransform();
        //    transform.m_rotation = transform.m_rotation * mathfu::Quat::FromEulerAngles(mathfu::Vector3(0.1f * deltaTime));
        //}

        // ------
        // Render
        // ------
        const mathfu::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
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

        auto commandList = device->CreateCommandList();

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
            std::future drawTriangle = std::async(std::launch::async, [&]()
                {

                    const mathfu::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
                    commandList->ClearFrameBuffer(*frameBuffer, clearColor);

                    //commandList->BindFrameBuffer(*frameBuffer);
                    //commandList->BindViewport(mathfu::Vector2(0.0f, 0.0f), mathfu::Vector2(window->GetSize()));

                    //commandList->BindPipeline();

                    //commandList->VertexBuffer();
                    //commandList->IndexBuffer();
                    //commandList->Resource();

                    //commandList->DrawIndexed();

                    commandList->FinishCommandList();
                });

            drawTriangle.wait();

            device->ExecuteCommandLists({ commandList.get() });

            swapChain->Present();
        }
    }

    DX::DeviceManager::Destroy();
    DX::WindowManager::Destroy();

#endif // SIMPLE_RENDERER

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
