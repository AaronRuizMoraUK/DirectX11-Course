#include <Renderer/Renderer.h>
#include <Window/Window.h>
#include <File/FileUtils.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <array>

namespace DX
{
    Renderer::Renderer(RendererId rendererId, Window* window)
        : m_rendererId(rendererId)
        , m_window(window)
    {
    }

    Renderer::~Renderer()
    {
        Terminate();
    }

    bool Renderer::Initialize()
    {
        if (m_device)
        {
            return true; // Already initialized
        }

        DX_LOG(Info, "Renderer", "Initializing DX11 Renderer...");

        if (!CreateDevice())
        {
            Terminate();
            return false;
        }

        if (!CreateSwapChain())
        {
            Terminate();
            return false;
        }

        if (!CreateRenderTargetView())
        {
            Terminate();
            return false;
        }

        if (!CreateShaders())
        {
            Terminate();
            return false;
        }

        if (!CreateInputLayout())
        {
            Terminate();
            return false;
        }

        return true;
    }

    void Renderer::Terminate()
    {
        DX_LOG(Info, "Renderer", "Terminating DX11 Renderer...");

        DestroyInputLayout();
        DestroyShaders();

        m_renderTargetView.Reset();
        m_swapChain.Reset();
        m_device.Reset();
    }

    Window* Renderer::GetWindow()
    {
        return m_window;
    }

    ComPtr<ID3D11Device> Renderer::GetDevice()
    {
        return m_device;
    }

    ComPtr<ID3D11DeviceContext> Renderer::GetDeviceContext()
    {
        return m_deviceContext;
    }

    bool Renderer::CreateDevice()
    {
        const std::array<D3D_FEATURE_LEVEL, 1> featureLevels = { D3D_FEATURE_LEVEL_11_1 };

        auto result = D3D11CreateDevice(
            nullptr, // IDXIGAdapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // Software
            0, // Flags
            featureLevels.data(),
            featureLevels.size(),
            D3D11_SDK_VERSION,
            m_device.GetAddressOf(),
            nullptr, // Feature level selected
            m_deviceContext.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Error, "Renderer", "Failed to create D3D11 device.");
            return false;
        }

        DX_LOG(Info, "Renderer", "D3D11 device created.");
        return true;
    }

    bool Renderer::CreateSwapChain()
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = m_window->GetSize().x;
        swapChainDesc.BufferDesc.Height = m_window->GetSize().y;
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = m_window->GetWindowNativeHandler();
        swapChainDesc.Windowed = true;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        ComPtr<IDXGIDevice> dxgiDevice;
        m_device.As(&dxgiDevice);

        ComPtr<IDXGIAdapter> dxgiAdapter;
        dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());

        ComPtr<IDXGIFactory> dxgiFactory;
        dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

        auto result = dxgiFactory->CreateSwapChain(m_device.Get(), &swapChainDesc, m_swapChain.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "Renderer", "Failed to create D3D11 swap chain.");
            return false;
        }

        DX_LOG(Info, "Renderer", "D3D11 swap chain created.");
        return true;
    }

    bool Renderer::CreateRenderTargetView()
    {
        ComPtr<ID3D11Texture2D> colorBuffer;
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(colorBuffer.GetAddressOf()));

        auto result = m_device->CreateRenderTargetView(
            colorBuffer.Get(),
            nullptr, // Description
            m_renderTargetView.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Error, "Renderer", "Failed to create Render Target View for swap chain's buffer.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Render Target View for swap chain's buffer created.");
        return true;
    }

    void Renderer::ClearColor(const mathfu::Color& color)
    {
        m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), mathfu::ColorPacked(color).data_);
    }

    void Renderer::Present()
    {
        m_swapChain->Present(
            1, // VSync On
            0  // Flags
        );
    }

    void Renderer::SetPipeline()
    {
        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
        m_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
        m_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
        m_deviceContext->IASetInputLayout(m_inputLayout.Get());
        m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(m_window->GetSize().x);
        viewport.Height = static_cast<float>(m_window->GetSize().y);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_deviceContext->RSSetViewports(1, &viewport);
    }

    void Renderer::Draw(int indexCount)
    {
        m_deviceContext->DrawIndexed(indexCount, 0, 0);
    }

    bool Renderer::CreateShaders()
    {
        const std::string vertexShaderFilename("Shaders/VertexShader.hlsl");
        m_vertexShaderBlob = CompileShader(vertexShaderFilename, "main", "vs_5_0");
        if (!m_vertexShaderBlob)
        {
            return false;
        }

        auto resultVS = m_device->CreateVertexShader(
            m_vertexShaderBlob->GetBufferPointer(),
            m_vertexShaderBlob->GetBufferSize(),
            nullptr, // Class linkage
            m_vertexShader.GetAddressOf());

        if (FAILED(resultVS))
        {
            DX_LOG(Error, "Renderer", "Failed to create vertex shader %s.", vertexShaderFilename.c_str());
            return false;
        }

        const std::string pixelShaderFilename("Shaders/PixelShader.hlsl");
        m_pixelShaderBlob = CompileShader(pixelShaderFilename, "main", "ps_5_0");
        if (!m_pixelShaderBlob)
        {
            return false;
        }

        auto resultPS = m_device->CreatePixelShader(
            m_pixelShaderBlob->GetBufferPointer(),
            m_pixelShaderBlob->GetBufferSize(),
            nullptr, // Class linkage
            m_pixelShader.GetAddressOf());

        if (FAILED(resultPS))
        {
            DX_LOG(Error, "Renderer", "Failed to create pixel shader %s.", pixelShaderFilename.c_str());
            return false;
        }

        return true;
    }

    void Renderer::DestroyShaders()
    {
        m_vertexShader.Reset();
        m_pixelShader.Reset();
        m_vertexShaderBlob.Reset();
        m_pixelShaderBlob.Reset();
    }

    ComPtr<ID3DBlob> Renderer::CompileShader(const std::string& shaderFilename, const std::string& entryPoint, const std::string& shaderModel) const
    {
        const auto pixelShaderCode = ReadAssetFile(shaderFilename);
        if (!pixelShaderCode.has_value())
        {
            return {};
        }

        ComPtr<ID3DBlob> shaderBlob;
        ComPtr<ID3DBlob> errorBlob;

        auto result = D3DCompile(
            pixelShaderCode->c_str(),
            pixelShaderCode->length(),
            nullptr, // Source name
            nullptr, // Macros for shader
            nullptr, // Includes for shader
            entryPoint.c_str(),
            shaderModel.c_str(),
            D3DCOMPILE_ENABLE_STRICTNESS, // Flags 1
            0, // Flags 2
            shaderBlob.GetAddressOf(),
            errorBlob.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "Renderer", "Failed to compile shader %s.", shaderFilename.c_str());
            if (errorBlob && errorBlob->GetBufferPointer())
            {
                DX_LOG(Error, "Renderer", "Message from shader compiler:\n%s\n", static_cast<char*>(errorBlob->GetBufferPointer()));
            }
            return {};
        }

        return shaderBlob;
    }

    bool Renderer::CreateInputLayout()
    {
        const std::array<D3D11_INPUT_ELEMENT_DESC, 2> inputLayoutDesc =
        {{
            { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        }};

        // Once an input-layout object is created from a vertex shader signature, the input-layout object can be reused
        // with any other vertex shader that has an identical input signature (semantics included). 
        auto result = m_device->CreateInputLayout(
            inputLayoutDesc.data(),
            inputLayoutDesc.size(),
            m_vertexShaderBlob->GetBufferPointer(),
            m_vertexShaderBlob->GetBufferSize(),
            m_inputLayout.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "Renderer", "Failed to create input layout.");
            return false;
        }

        return true;
    }

    void Renderer::DestroyInputLayout()
    {
        m_inputLayout.Reset();
    }
} // namespace DX
