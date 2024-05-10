#pragma once

#include <Renderer/Renderer.h>

#include <memory>
#include <unordered_map>

class Window;

class RendererManager
{
public:
    static RendererManager& Get();
    static void Destroy();

    ~RendererManager() = default;

    // Delete copy constructor and assignment operator to prevent copying
    RendererManager(const RendererManager&) = delete;
    RendererManager& operator=(const RendererManager&) = delete;

    Renderer* CreateRenderer(Window* window);
    void DestroyRenderer(RendererId rendererId);

    Renderer* GetRenderer(RendererId rendererId);

private:
    RendererManager() = default;

    static std::unique_ptr<RendererManager> Instance;
    static RendererId NextRendererId;

    using Renderers = std::unordered_map<RendererId, std::unique_ptr<Renderer>>;
    Renderers m_renderers;
};
