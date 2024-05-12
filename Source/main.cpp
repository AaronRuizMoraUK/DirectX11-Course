#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>

#include <cstdio>
#include <array>
#include <memory>

namespace
{
    const std::array<DX::Vertex, 3> TriangleVertexData = 
    {{
        { mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Colors::RedPacked },
        { mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Colors::GreenPacked },
        { mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Colors::BluePacked }
    }};

    const std::array<DX::Vertex, 3> TriangleVertexData2 =
    {{
        { mathfu::Vector3Packed(mathfu::Vector3(0.6f, 0.0f, 0.0f)), mathfu::Colors::RedPacked },
        { mathfu::Vector3Packed(mathfu::Vector3(0.8f, 0.7f, 0.0f)), mathfu::Colors::GreenPacked },
        { mathfu::Vector3Packed(mathfu::Vector3(1.0f, 0.0f, 0.0f)), mathfu::Colors::BluePacked }
    }};

    const std::array<uint32_t, 3> TriangleIndexData = { 0, 1, 2 };
    
    std::array<DX::Vertex, 8> CubeVertexData(const mathfu::Vector3& extends)
    {
        const mathfu::Vector3 half = 0.5f * extends;
        return
            {{
                { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, -half.z)), mathfu::Colors::RedPacked },
                { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, half.z)), mathfu::Colors::GreenPacked },
                { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y, half.z)), mathfu::Colors::BluePacked },
                { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y, -half.z)), mathfu::Colors::CyanPacked },

                { mathfu::Vector3Packed(mathfu::Vector3(-half.x, half.y, -half.z)), mathfu::Colors::MagentaPacked },
                { mathfu::Vector3Packed(mathfu::Vector3(-half.x, half.y, half.z)), mathfu::Colors::YellowPacked },
                { mathfu::Vector3Packed(mathfu::Vector3(half.x, half.y, half.z)), mathfu::Colors::WhitePacked },
                { mathfu::Vector3Packed(mathfu::Vector3(half.x, half.y, -half.z)), mathfu::Colors::BlackPacked },
            }};
    }

    const std::array<uint32_t, 6*2*3> CubeIndexData = // 6 faces, 2 triangles each face, 3 vertices each triangle.
    { 
        2, 1, 0, 
        0, 3, 2,

        4, 5, 6, 
        6, 7, 4,

        1, 2, 6, 
        6, 5, 1,

        2, 3, 7, 
        7, 6, 2,

        3, 0, 4, 
        4, 7, 3,

        0, 1, 5, 
        5, 4, 0
    };
}

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
    objects.push_back(std::make_unique<DX::Object>(TriangleVertexData, TriangleIndexData));
    objects.push_back(std::make_unique<DX::Object>(TriangleVertexData2, TriangleIndexData));
    objects.push_back(std::make_unique<DX::Object>(CubeVertexData(mathfu::Vector3(1.0f)), CubeIndexData));
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
        //for (auto& object : objects)
        //{
        //    mathfu::Transform& transform = object->GetTransform();
        //    transform.m_rotation = transform.m_rotation * mathfu::Quat::FromEulerAngles(mathfu::Vector3(0.0f, 0.0, 1.0f * deltaTime));
        //}

        // ------
        // Render
        // ------
        renderer->ClearColor(mathfu::Colors::Magenta * 0.3f);

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
