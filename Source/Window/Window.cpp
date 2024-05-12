#include <Window/Window.h>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#endif

#include <cstdio>

namespace DX
{
    Window::Window(WindowId windowId, const WindowSize& size, std::string title)
        : m_windowId(windowId)
        , m_size(size)
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

        // Do not use any client API and make it not resizable.
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        std::printf("Creating window %u with size %dx%d...\n", m_windowId, m_size.m_width, m_size.m_height);
        m_window = glfwCreateWindow(m_size.m_width, m_size.m_height, m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            std::printf("Error: Failed to create GLFW window.\n");
            return false;
        }

        // Hide mouse cursor
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Accumulate mouse scroll offset
        glfwSetWindowUserPointer(m_window, this);
        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                if (auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
                {
                    self->m_scrollOffsetAccumulator += static_cast<float>(yoffset);
                }
            });

        return true;
    }

    void Window::Terminate()
    {
        if (m_window)
        {
            std::printf("Terminating window %u...\n", m_windowId);
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    bool Window::IsVisible() const
    {
        return !glfwWindowShouldClose(m_window);
    }

    HWND Window::GetWindowNativeHandler()
    {
#ifdef _WIN32
        return glfwGetWin32Window(m_window);
#else
#error "Window::GetWindowNativeHandler: Unsupported platform."
        return NULL;
#endif
    }

    void Window::PollEvents()
    {
        // Set scroll offset and reset accumulator
        m_scrollOffset = m_scrollOffsetAccumulator;
        m_scrollOffsetAccumulator = 0.0f;
    }
} // namespace DX
