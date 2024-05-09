#pragma once

#include <Math/Vector4.h>

// For COM objects' smart pointers
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class Window;
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class Renderer
{
public:
    Renderer(Window& window);
    ~Renderer();

    bool Initialize();
    void Terminate();

    void ClearColor(const mathfu::Vector4& color);
    void Present();

protected:
    bool CreateDevice();
    bool CreateSwapChain();
    bool CreateRenderTargetView();

private:
    Window& m_window;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};
