#pragma once

#include <GenericId/GenericId.h>
#include <Math/Vector2.h>

#include <string>

struct GLFWwindow;
typedef struct HWND__* HWND;

namespace DX
{
    using WindowId = GenericId<struct WindowIdTag>;

    class Window
    {
    public:
        Window(WindowId windowId, std::string title, const Math::Vector2Int& size, int refreshRate, bool fullScreen, bool vSync);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool Initialize();
        void Terminate();

        WindowId GetId() const { return m_windowId; }

        bool IsOpen() const;
        const Math::Vector2Int& GetSize() const { return m_size; }
        int GetRefreshRate() const { return m_refreshRate; }
        bool IsFullScreen() const { return m_fullScreen; }
        bool IsVSyncEnabled() const { return m_vSync; }

        GLFWwindow* GetWindowHandler() { return m_window; }
        HWND GetWindowNativeHandler();

        float GetScrollOffset() const { return m_scrollOffset; }

        // Called by WindowManager::PollEvents
        void PollEvents();

    private:
        const WindowId m_windowId;
        std::string m_title;
        Math::Vector2Int m_size;
        int m_refreshRate = 60;
        bool m_fullScreen = false;
        bool m_vSync = true;

        GLFWwindow* m_window = nullptr;

        // Mouse scroll
        float m_scrollOffset = 0.0f;
        float m_scrollOffsetAccumulator = 0.0f;
    };
} // namespace DX
