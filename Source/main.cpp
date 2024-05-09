#include <Window/Window.h>
#include <Renderer/Renderer.h>

#include <cstdio>

int main()
{
    Window window({1280, 720}, "DirectX11 Course");

    if (!window.Initialize())
    {
        return -1;
    }

    Renderer renderer(window);

    while (window.IsVisible())
    {
        window.Run();
    }

    window.Terminate();

    std::printf("Done!\n");
    return 0;
}
