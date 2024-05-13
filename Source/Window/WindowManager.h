#pragma once

#include <Window/Window.h>

#include <memory>
#include <unordered_map>

namespace DX
{
    class WindowManager
    {
    public:
        static WindowManager& Get();
        static void Destroy();

        ~WindowManager();

        // Delete copy constructor and assignment operator to prevent copying
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

        Window* CreateWindowWithTitle(const mathfu::Vector2Int& size, std::string title);
        void DestroyWindow(WindowId windowId);

        Window* GetWindow(WindowId windowId);

        void PollEvents();

    private:
        WindowManager();

        static std::unique_ptr<WindowManager> Instance;
        static WindowId NextWindowId;

        using Windows = std::unordered_map<WindowId, std::unique_ptr<Window>>;
        Windows m_windows;
    };
} // namespace DX