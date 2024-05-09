#pragma once

#include <string>

struct GLFWwindow;
typedef struct HWND__* HWND;

struct WindowSize
{
    int m_width = 0;
    int m_height = 0;
};

class Window
{
public:
    Window(WindowSize size, std::string title);
    ~Window();

    bool Initialize();
    void Run();
    void Terminate();

    bool IsVisible() const;
    WindowSize GetSize() const { return m_size; }

    GLFWwindow* GetWindowHandler() { return m_window; }
    HWND GetWindowNativeHandler();

private:
    GLFWwindow* m_window = nullptr;
    WindowSize m_size;
    std::string m_title;
};
