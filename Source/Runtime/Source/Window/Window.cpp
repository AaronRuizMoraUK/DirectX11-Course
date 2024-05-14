#include <Window/Window.h>
#include <Log/Log.h>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#endif

namespace DX
{
    Window::Window(WindowId windowId, const mathfu::Vector2Int& size, std::string title)
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

        DX_LOG(Info, "Window", "Creating window %u with size %dx%d...", m_windowId, m_size.x, m_size.y);
        m_window = glfwCreateWindow(m_size.x, m_size.y, m_title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            DX_LOG(Error, "Window", "Failed to create GLFW window.");
            return false;
        }

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
            DX_LOG(Info, "Window", "Terminating window %u...", m_windowId);
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
