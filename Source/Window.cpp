
#include "Window.h"

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <cstdio>

Window::Window(WindowSize size, std::string title)
    : m_size(std::move(size))
    , m_title(std::move(title))
{
}

Window::~Window()
{
    Terminate();
}

bool Window::Initialize()
{
    if (m_window)
    {
        return true; // Already initialized
    }

    std::printf("Initializing GLFW...\n");
    if (!glfwInit())
    {
        std::printf("Failed to initialize GLFW.\n");
        return false;
    }

    // Do not use any client API and make it not resizable.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    std::printf("Creating window of size %dx%d...\n", m_size.m_width, m_size.m_height);
    m_window = glfwCreateWindow(m_size.m_width, m_size.m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        std::printf("Failed to create GLFW window.\n");
        glfwTerminate();
        return false;
    }

    return true;
}

void Window::Run()
{
    if (m_window)
    {
        glfwPollEvents();
    }
}

void Window::Terminate()
{
    if (m_window)
    {
        std::printf("Terminating GLFW...\n");
        glfwTerminate();
        m_window = nullptr;
    }
}

bool Window::IsVisible() const
{
    return m_window && !glfwWindowShouldClose(m_window);
}

HWND Window::GetWindowNativeHandler()
{
    if (m_window)
    {
        return glfwGetWin32Window(m_window);
    }
    return NULL;
}
