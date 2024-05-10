#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>

#include <cstdio>
#include <array>

static const std::array<DX::Vertex, 3> VertexData = 
{{
    { mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Colors::RedPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Colors::GreenPacked },
    { mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Colors::BluePacked }
}};

static const std::array<uint32_t, 3> IndexData = { 0, 1, 2 };

int main()
{
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle(DX::WindowSize{1280, 720}, "DirectX11 Course");
    if (!window)
    {
        return -1;
    }

    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.CreateRenderer(window);
    if (!renderer)
    {
        return -1;
    }

    DX::Object triangle(VertexData, IndexData);

    while (window->IsVisible())
    {
        windowManager.PollEvents();

        renderer->ClearColor(mathfu::Colors::Black);

        renderer->SetPipeline();

        triangle.SetBuffers();

        renderer->Draw(triangle.GetIndexCount());

        renderer->Present();
    }

    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();

    std::printf("Done!\n");
    return 0;
}
