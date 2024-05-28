#pragma once

#include <GenericId/GenericId.h>
#include <Math/Color.h>

#include <memory>

namespace DX
{
    class Window;
    class Device;
    class SwapChain;
    class FrameBuffer;
    class Scene;

    using RendererId = GenericId<struct RendererIdTag>;

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

        void Clear(const Math::Color& color, float depth, uint8_t stencil);
        void Present();

    private:
        bool CreateDevice();
        bool CreateSwapChainAndFrameBuffer();
        bool CreateScene();

        RendererId m_rendererId;
        Window* m_window = nullptr;
        std::unique_ptr<Device> m_device;
        std::shared_ptr<SwapChain> m_swapChain;
        std::shared_ptr<FrameBuffer> m_frameBuffer;
        std::unique_ptr<Scene> m_scene;
    };
} // namespace DX
