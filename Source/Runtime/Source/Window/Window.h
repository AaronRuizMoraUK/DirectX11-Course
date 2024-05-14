#pragma once

#include <Math/Vector2.h>

#include <string>

struct GLFWwindow;
typedef struct HWND__* HWND;

namespace DX
{
    namespace Internal
    {
        template<typename Tag>
        using WindowIdType = uint32_t;
    }
    using WindowId = Internal::WindowIdType<struct WindowIdTag>;
    static const WindowId InvalidWindowId = 0xFFFFFFFF;

    class Window
    {
    public:
        Window(WindowId windowId, std::string title, const mathfu::Vector2Int& size, int refreshRate, bool fullScreen, bool vSync);
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool Initialize();
        void Terminate();

        bool IsOpen() const;
        const mathfu::Vector2Int& GetSize() const { return m_size; }
        int GetRefreshRate() const { return m_refreshRate; }
        bool IsFullScreen() const { return m_fullScreen; }
        bool IsVSyncEnabled() const { return m_vSync; }

        WindowId GetId() const { return m_windowId; }
        GLFWwindow* GetWindowHandler() { return m_window; }
        HWND GetWindowNativeHandler();

        float GetScrollOffset() const { return m_scrollOffset; }

        // Called by WindowManager::PollEvents
        void PollEvents();

    private:
        const WindowId m_windowId = InvalidWindowId;
        std::string m_title;
        mathfu::Vector2Int m_size;
        int m_refreshRate = 60;
        bool m_fullScreen = false;
        bool m_vSync = true;

        GLFWwindow* m_window = nullptr;

        // Mouse scroll
        float m_scrollOffset = 0.0f;
        float m_scrollOffsetAccumulator = 0.0f;
    };
} // namespace DX
