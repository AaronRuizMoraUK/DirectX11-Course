#include <Renderer/RendererManager.h>
#include <Renderer/Scene.h>
#include <Renderer/Object.h>
#include <Camera/Camera.h>

#include <Window/WindowManager.h>
#include <Assets/AssetManager.h>
#include <Math/Vector3.h>
#include <Math/Color.h>
#include <Log/Log.h>

#include <memory>
#include <chrono>
#include <thread>
#include <future>

int main()
{
    //const Math::Vector2Int windowSize{ 3440, 1440 };
    //const int refreshRate = 144;
    //const bool fullScreen = true;
    //const bool vSync = true;

    const Math::Vector2Int windowSize{ 1280, 720 };
    const int refreshRate = 60;
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

    {
        // Camera
        auto camera = std::make_unique<DX::Camera>(Math::Vector3(0.0f, 1.0f, -2.0f), Math::Vector3(0.0f));

        // Render objects
        std::vector<std::unique_ptr<DX::Object>> objects;
        objects.push_back(std::make_unique<DX::Cube>(Math::Vector3(1.0f)));
        objects.push_back(std::make_unique<DX::Mesh>(
            "Models/DamagedHelmet/DamagedHelmet.gltf", 
            "Models/DamagedHelmet/Default_albedo.jpg",
            "Models/DamagedHelmet/Default_normal.jpg"));
        objects.push_back(std::make_unique<DX::Mesh>(
            "Models/Jack/Jack.fbx", 
            "Textures/Wall_Stone_Albedo.png",
            "Textures/Wall_Stone_Normal.png"));
        objects[0]->SetTransform(Math::Vector3(-1.5f, 0.0f, 0.0f));
        objects[1]->SetTransform({ Math::Vector3(2.0f, 0.0f, 0.0f), Math::Quaternion::FromEulerAngles({ -1.57f, 0.0f, 0.0f }) });
        objects[2]->SetTransform({ Math::Vector3(0.0f, -1.0f, 0.0f), Math::Quaternion::FromEulerAngles({0.0f, 3.14f, 0.0f}), Math::Vector3(0.01f) });

        // Render Scene
        DX::Scene* scene = renderer->GetScene();
        scene->SetCamera(camera.get());
        std::ranges::for_each(objects, [scene](auto& object) { scene->AddObject(object.get()); });

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
            for (auto& object : objects)
            {
                Math::Transform& transform = object->GetTransform();
                transform.m_rotation = Math::Quaternion::FromEulerAngles(Math::Vector3(0.0f, 0.3f * deltaTime, 0.0f)) * transform.m_rotation;
            }

            // ------
            // Render
            // ------
            const Math::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
            renderer->Clear(clearColor, 1.0f, 0);

            scene->Render();

            scene->WaitAndExecute();

            renderer->Present();
        }
    }

    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();
    DX::AssetManager::Destroy();

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
