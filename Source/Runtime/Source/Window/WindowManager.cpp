#include <Window/WindowManager.h>
#include <Log/Log.h>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <ranges>

namespace DX
{
    std::unique_ptr<WindowManager> WindowManager::Instance;

    WindowId WindowManager::NextWindowId = 0;

    WindowManager& WindowManager::Get()
    {
        if (!Instance)
        {
            Instance.reset(new WindowManager());
        }
        return *Instance;
    }

    void WindowManager::Destroy()
    {
        Instance.reset();
    }

    WindowManager::WindowManager()
    {
        DX_LOG(Info, "Window Manager", "Initializing GLFW...");
        bool success = glfwInit();
        DX_ASSERT(success, "Window Manager", "Failed to initialize GLFW.");
    }

    WindowManager::~WindowManager()
    {
        // Clearing the map will destroy all windows.
        m_windows.clear();

        DX_LOG(Info, "Window Manager", "Terminating GLFW...");
        glfwTerminate();
    }

    Window* WindowManager::CreateWindowWithTitle(const mathfu::Vector2Int& size, std::string title)
    {
        auto newWindow = std::make_unique<Window>(NextWindowId, size, std::move(title));
        if (!newWindow->Initialize())
        {
            return nullptr;
        }

        auto result = m_windows.emplace(NextWindowId, std::move(newWindow));
        ++NextWindowId;

        // Result's first is the iterator to the newly inserted element (pair),
        // its second is the value of the element (unique_ptr<Window>).
        return result.first->second.get();
    }

    void WindowManager::DestroyWindow(WindowId windowId)
    {
        // Removing the window from the map will destroy it.
        m_windows.erase(windowId);
    }

    Window* WindowManager::GetWindow(WindowId windowId)
    {
        if (auto it = m_windows.find(windowId);
            it != m_windows.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void WindowManager::PollEvents()
    {
        glfwPollEvents();

        for (auto& window : m_windows | std::views::values)
        {
            window->PollEvents();
        }
    }
} // namespace DX