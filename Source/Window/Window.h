#pragma once

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

    struct WindowSize
    {
        uint32_t m_width = 0;
        uint32_t m_height = 0;
    };

    class Window
    {
    public:
        Window(WindowId windowId, const WindowSize& size, std::string title);
        ~Window();

        bool Initialize();
        void Terminate();

        bool IsVisible() const;
        const WindowSize& GetSize() const { return m_size; }

        WindowId GetId() const { return m_windowId; }
        GLFWwindow* GetWindowHandler() { return m_window; }
        HWND GetWindowNativeHandler();

    private:
        const WindowId m_windowId = InvalidWindowId;
        WindowSize m_size;
        std::string m_title;
        GLFWwindow* m_window = nullptr;
    };
} // namespace DX
