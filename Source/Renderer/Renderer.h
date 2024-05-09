#pragma once

#include <Math/Vector4.h>

#include <string>
#include <filesystem>

// For COM objects' smart pointers
#include <wrl.h>

using Microsoft::WRL::ComPtr;

class Window;
class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

class ID3D11VertexShader;
class ID3D11PixelShader;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

class Renderer
{
public:
    Renderer(Window& window);
    ~Renderer();

    bool Initialize();
    void Terminate();

    void ClearColor(const mathfu::Vector4& color);
    void Present();

private:
    bool CreateDevice();
    bool CreateSwapChain();
    bool CreateRenderTargetView();

    Window& m_window;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_deviceContext;
    ComPtr<IDXGISwapChain> m_swapChain;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;

private:
    bool CreateShaders();
    void DestroyShaders();

    std::string ReadAssetFile(const std::string& fileName) const;
    std::filesystem::path GetAssetPath() const;
    std::filesystem::path GetExecutablePath() const;

    ComPtr<ID3D11VertexShader> m_vertexShader;
    ComPtr<ID3DBlob> m_vertexShaderBlob;
    ComPtr<ID3D11PixelShader> m_pixelShader;
};
