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

        // First window created will become the default one.
        // Default window cannot be destroyed with DestroyWindow.
        static inline const WindowId DefaultWindowId{ 1 };

        ~WindowManager();

        // Delete copy constructor and assignment operator to prevent copying
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

        Window* CreateWindowWithTitle(std::string title, const mathfu::Vector2Int& size, int refreshRate = 60, bool fullScreen = false, bool vSync = false);
        void DestroyWindow(WindowId windowId);

        Window* GetWindow(WindowId windowId = DefaultWindowId);

        void PollEvents();

    private:
        WindowManager();

        static std::unique_ptr<WindowManager> Instance;
        static WindowId NextWindowId;

        using Windows = std::unordered_map<WindowId, std::unique_ptr<Window>>;
        Windows m_windows;
    };
} // namespace DX