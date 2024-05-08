
#include "Window.h"

Window::Window(WindowSize size, std::string title)
    : m_size(std::move(size))
    , m_title(std::move(title))
{
}

Window::~Window()
{
    // Terminate GLFW.
    printf("Terminating GLFW...\n");
    if (m_window)
    {
        glfwTerminate();
    }
}

bool Window::Initialize()
{
    // Initialize GLFW.
    printf("Initializing GLFW...\n");
    if (!glfwInit())
    {
        printf("Failed to initialize GLFW.\n");
        return false;
    }

    // Do not use any client API and make it not resizable.
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create our window
    printf("Creating window of size %dx%d...\n", m_size.m_width, m_size.m_height);
    m_window = glfwCreateWindow(m_size.m_width, m_size.m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        printf("Failed to create GLFW window.\n");
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
