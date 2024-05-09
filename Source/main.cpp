#include <Window/Window.h>
#include <Renderer/Renderer.h>

#include <cstdio>
#include <memory>

int main()
{
    auto window = std::make_unique<Window>(WindowSize{1280, 720}, "DirectX11 Course");
    if (!window->Initialize())
    {
        return -1;
    }

    auto renderer = std::make_unique<Renderer>(*window);
    if (!renderer->Initialize())
    {
        return -1;
    }

    while (window->IsVisible())
    {
        window->Run();

        renderer->ClearColor({0.0f, 0.0f, 0.3f, 1.0f});

        renderer->Present();
    }

    renderer.reset();
    window.reset();

    std::printf("Done!\n");
    return 0;
}
