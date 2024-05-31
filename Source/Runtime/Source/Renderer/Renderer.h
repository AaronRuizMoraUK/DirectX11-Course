#pragma once

#include <Window/Window.h>
#include <GenericId/GenericId.h>
#include <Math/Color.h>

#include <memory>

namespace DX
{
    class Device;
    class SwapChain;
    class FrameBuffer;
    class Scene;

    using RendererId = GenericId<struct RendererIdTag>;

    // Manages the render device, swap chain, frame buffer and scene.
    class Renderer
    {
    public:
        Renderer(RendererId rendererId, Window* window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        bool Initialize();
        void Terminate();

        RendererId GetId() const { return m_rendererId; }

        Window* GetWindow();
        Device* GetDevice();
        FrameBuffer* GetFrameBuffer();
        Scene* GetScene();

        void Present();

    private:
        bool CreateDevice();
        bool CreateSwapChain();
        bool CreateScene();

        RendererId m_rendererId;
        Window* m_window = nullptr;
        WindowResizeEvent::Handler m_windowResizeHandler;
        std::unique_ptr<Device> m_device;
        std::shared_ptr<SwapChain> m_swapChain;
        std::unique_ptr<Scene> m_scene;
    };
} // namespace DX
