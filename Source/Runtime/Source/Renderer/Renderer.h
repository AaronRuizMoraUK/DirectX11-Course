#pragma once

#include <Math/Color.h>

#include <string>
#include <filesystem>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Device;
class ID3D11DeviceContext;
class IDXGISwapChain;
class ID3D11RenderTargetView;

struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
class ID3D11VertexShader;
class ID3D11PixelShader;
class ID3D11InputLayout;

namespace DX
{
    class Window;

    namespace Internal
    {
        template<typename Tag>
        using RendererIdType = uint32_t;
    }
    using RendererId = Internal::RendererIdType<struct RendererIdTag>;
    static const RendererId InvalidRendererId = 0xFFFFFFFF;

    class Renderer
    {
    public:
        Renderer(RendererId rendererId, Window* window);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;

        bool Initialize();
        void Terminate();

        Window* GetWindow();
        ComPtr<ID3D11Device> GetDevice();
        ComPtr<ID3D11DeviceContext> GetDeviceContext();

        void ClearColor(const Math::Color& color);
        void Present();

        void SetPipeline();

        void Draw(int indexCount);

    private:
        bool CreateDevice();
        bool CreateSwapChain();
        bool CreateRenderTargetView();

        const RendererId m_rendererId = InvalidRendererId;
        Window* m_window = nullptr;

        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_deviceContext;
        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    private:
        bool CreateShaders();
        void DestroyShaders();

        ComPtr<ID3DBlob> CompileShader(const std::string& shaderFilename, const std::string& entryPoint, const std::string& shaderModel) const;

        ComPtr<ID3DBlob> m_vertexShaderBlob;
        ComPtr<ID3DBlob> m_pixelShaderBlob;
        ComPtr<ID3D11VertexShader> m_vertexShader;
        ComPtr<ID3D11PixelShader> m_pixelShader;

    private:
        bool CreateInputLayout();
        void DestroyInputLayout();

        ComPtr<ID3D11InputLayout> m_inputLayout;
    };
} // namespace DX
