#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>
#include <Log/Log.h>

#include <array>
#include <memory>

int main()
{
    //const mathfu::Vector2Int windowSize{ 3440, 1440 };
    //const int refreshRate = 144;
    //const bool fullscreen = true;
    //const bool vSync = true;

    const mathfu::Vector2Int windowSize{ 1280, 720 };
    const int refreshRate = 60;
    const bool fullscreen = false;
    const bool vSync = true;

    // Window initialization
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle("DirectX11 Course", windowSize, refreshRate, fullscreen, vSync);
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
        const mathfu::Vector4 clearColor(0.2f, 0.0f, 0.3f, 1.0f);
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

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
