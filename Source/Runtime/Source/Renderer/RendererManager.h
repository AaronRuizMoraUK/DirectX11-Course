#pragma once

#include <Renderer/Renderer.h>

#include <memory>
#include <unordered_map>

namespace DX
{
    class Window;

    class RendererManager
    {
    public:
        static RendererManager& Get();
        static void Destroy();

        // First renderer created will become the default one.
        // Default renderer cannot be destroyed with DestroyRenderer.
        static inline const RendererId DefaultRendererId{ 1 };

        ~RendererManager() = default;

        // Delete copy constructor and assignment operator to prevent copying
        RendererManager(const RendererManager&) = delete;
        RendererManager& operator=(const RendererManager&) = delete;

        Renderer* CreateRenderer(Window* window);
        void DestroyRenderer(RendererId rendererId);

        Renderer* GetRenderer(RendererId rendererId = DefaultRendererId);

    private:
        RendererManager() = default;

        static std::unique_ptr<RendererManager> Instance;
        static RendererId NextRendererId;

        using Renderers = std::unordered_map<RendererId, std::unique_ptr<Renderer>>;
        Renderers m_renderers;
    };
} // namespace DX
