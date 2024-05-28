#pragma once

#include <Math/Matrix4x4.h>

#include <memory>
#include <unordered_set>
#include <future>

namespace DX
{
    class Renderer;
    class Camera;
    class Object;
    class PipelineObject;
    class CommandList;
    class Buffer;

    // A scene is a collection of objects and a camera.
    // It is responsible for rendering all the objects added to the scene.
    class Scene
    {
    public:
        Scene(Renderer* renderer);
        ~Scene();

        void SetCamera(Camera* camera);

        void AddObject(Object* object);
        void RemoveObject(Object* object);

        void Render();
        void WaitAndExecute();

    private:
        Renderer* m_renderer = nullptr;
        Camera* m_camera = nullptr;
        std::unique_ptr<PipelineObject> m_pipelineObject;

        std::unordered_set<Object*> m_objects;

        std::shared_ptr<CommandList> m_commandList;
        std::future<void> drawObjects;

        // Per Scene Resources
        struct ViewProjBuffer
        {
            Math::Matrix4x4Packed m_viewMatrix;
            Math::Matrix4x4Packed m_projMatrix;
        };
        std::shared_ptr<Buffer> m_viewProjMatrixConstantBuffer;

        // Per Object Resources
        std::shared_ptr<Buffer> m_worldMatrixConstantBuffer;
    };
} // namespace DX
