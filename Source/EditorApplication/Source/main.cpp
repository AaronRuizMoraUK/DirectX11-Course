#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>

#include <Window/WindowManager.h>
#include <Math/Vector3.h>
#include <Math/Color.h>
#include <Log/Log.h>

#include <memory>
#include <chrono>

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

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
