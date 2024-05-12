#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>

#include <cstdio>
#include <array>
#include <memory>

int main()
{
    // Window initialization
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle(DX::WindowSize{1280, 720}, "DirectX11 Course");
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
    auto camera = std::make_unique<DX::Camera>(mathfu::Vector3(0.0f, 0.0f, -3.0f), mathfu::Vector3(0.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Triangle>());
    objects.push_back(std::make_unique<DX::Cube>(mathfu::Vector3(1.0f)));
    objects[0]->SetTransform(mathfu::Vector3(0.0f, 1.0f, 0.0f));

    while (window->IsVisible())
    {
        windowManager.PollEvents();

        // ------
        // Update
        // ------
        constexpr float deltaTime = 1.0f / 60.0f;
        camera->Update(deltaTime);
        for (auto& object : objects)
        {
            mathfu::Transform& transform = object->GetTransform();
            transform.m_rotation = transform.m_rotation * mathfu::Quat::FromEulerAngles(mathfu::Vector3(0.0f, 0.0, 1.0f * deltaTime));
        }

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

    std::printf("Done!\n");
    return 0;
}
