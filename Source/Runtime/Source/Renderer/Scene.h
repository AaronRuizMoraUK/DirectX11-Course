#pragma once

#include <Math/Matrix4x4.h>
#include <Math/Vector3.h>

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

    private:
        void UpdateLightInfo();

        Renderer* m_renderer = nullptr;
        Camera* m_camera = nullptr;
        std::unique_ptr<PipelineObject> m_pipelineObject;

        std::unordered_set<Object*> m_objects;

        std::shared_ptr<CommandList> m_commandListScene;
        std::shared_ptr<CommandList> m_commandListObjects;

        // Per Scene Resources
        struct ViewProjBuffer
        {
            Math::Matrix4x4Packed m_viewMatrix;
            Math::Matrix4x4Packed m_projMatrix;
            Math::Vector4Packed m_camPos;
        };
        std::shared_ptr<Buffer> m_viewProjMatrixConstantBuffer;

        struct LightBuffer
        {
            Math::Vector4Packed m_lightDir;
            Math::Vector4Packed m_lightColor;
        };
        LightBuffer m_lightInfo;
        std::shared_ptr<Buffer> m_lightConstantBuffer;

        // Per Object Resources
        struct WorldBuffer
        {
            Math::Matrix4x4Packed m_worldMatrix;
            Math::Matrix4x4Packed m_inverseTransposeWorldMatrix;
        };
        std::shared_ptr<Buffer> m_worldMatrixConstantBuffer;
    };
} // namespace DX
