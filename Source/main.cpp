#include <Window/Window.h>
#include <Renderer/Renderer.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrix3x3.h>
#include <Math/Matrix4x4.h>
#include <Math/Quat.h>

#include <cstdio>

int main()
{
    Window window({1280, 720}, "DirectX11 Course");

    if (!window.Initialize())
    {
        return -1;
    }

    Renderer renderer(window);
    mathfu::Vector2 vector2;
    mathfu::Vector3 vector3;
    mathfu::Vector4 vector4;
    mathfu::Matrix3x3 matrix3x3;
    mathfu::Matrix4x4 matrix4x4;
    mathfu::Quat quat;

    while (window.IsVisible())
    {
        window.Run();
    }

    window.Terminate();

    std::printf("Done!\n");
    return 0;
}
