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
        Window(WindowId windowId, const mathfu::Vector2Int& size, std::string title);
        ~Window();

        bool Initialize();
        void Terminate();

        bool IsVisible() const;
        const mathfu::Vector2Int& GetSize() const { return m_size; }

        WindowId GetId() const { return m_windowId; }
        GLFWwindow* GetWindowHandler() { return m_window; }
        HWND GetWindowNativeHandler();

        float GetScrollOffset() const { return m_scrollOffset; }

        // Called by WindowManager::PollEvents
        void PollEvents();

    private:
        const WindowId m_windowId = InvalidWindowId;
        mathfu::Vector2Int m_size;
        std::string m_title;
        GLFWwindow* m_window = nullptr;
        float m_scrollOffset = 0.0f;
        float m_scrollOffsetAccumulator = 0.0f;
    };
} // namespace DX
