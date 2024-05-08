#pragma once

#include <string>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

struct WindowSize
{
    int m_width = 0;
    int m_height = 0;
};

class Window
{
public:
    Window(WindowSize size, std::string title);
    ~Window();

    bool Initialize();
    void Run();

    bool IsVisible() const;
    WindowSize GetSize() const { return m_size; }

    GLFWwindow* GetWindowHandler() { return m_window; }
    HWND GetWindowNativeHandler();

private:
    GLFWwindow* m_window = nullptr;
    WindowSize m_size;
    std::string m_title;
};
