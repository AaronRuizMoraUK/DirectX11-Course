#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>

#include <cstdio>
#include <array>
#include <memory>

static const std::array<DX::Vertex, 3> VertexData1 = 
{{
    { mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Colors::RedPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Colors::GreenPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Colors::BluePacked }
}};

static const std::array<DX::Vertex, 3> VertexData2 = 
{{
    { mathfu::Vector3Packed(mathfu::Vector3(0.6f, 0.0f, 0.0f)), mathfu::Colors::RedPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(0.8f, 0.7f, 0.0f)), mathfu::Colors::GreenPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(1.0f, 0.0f, 0.0f)), mathfu::Colors::BluePacked }
}};

static const std::array<uint32_t, 3> IndexData = { 0, 1, 2 };

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
    auto camera = std::make_unique<DX::Camera>(mathfu::Vector3(0.0f, 0.0f, -5.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Object>(VertexData1, IndexData));
    objects.push_back(std::make_unique<DX::Object>(VertexData2, IndexData));
    objects[0]->SetTransform(mathfu::Vector3(0.0f, 1.0f, 0.0f));
    objects[1]->SetTransform(mathfu::Vector3(0.0f, 1.0f, 0.0f));

    while (window->IsVisible())
    {
        windowManager.PollEvents();

        // ------
        // Update
        // ------
        constexpr float deltaTime = 1.0f / 60.0f;
        camera->Update(deltaTime);

        // ------
        // Render
        // ------
        renderer->ClearColor(mathfu::Colors::Black);

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
