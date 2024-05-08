
#include <cstdio>

#include "Window.h"

int main()
{
    Window window({1280, 720}, "DirectX11 Course");

    if (window.Initialize())
    {
        return -1;
    }

    while (window.IsVisible())
    {
        window.Run();
    }

    printf("Done!\n");
    return 0;
}
