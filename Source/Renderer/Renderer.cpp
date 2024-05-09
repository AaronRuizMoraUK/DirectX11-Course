
#include "Renderer.h"

#include <Window/Window.h>

#include <d3d11.h>

#include <array>
#include <fstream>
#include <sstream>
#include <string_view>

Renderer::Renderer(Window& window)
    : m_window(window)
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

    std::printf("Initializing DX11 Renderer...\n");

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

    return true;
}

void Renderer::Terminate()
{
    std::printf("Terminating DX11 Renderer...\n");

    DestroyShaders();

    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_device.Reset();
}

bool Renderer::CreateDevice()
{
    std::array<D3D_FEATURE_LEVEL, 1> featureLevels = { D3D_FEATURE_LEVEL_11_1 };

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
        std::printf("Error: Failed to create D3D11 device.\n");
        return false;
    }

    std::printf("D3D11 device created.\n");
    return true;
}

bool Renderer::CreateSwapChain()
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferDesc.Width = m_window.GetSize().m_width;
    swapChainDesc.BufferDesc.Height = m_window.GetSize().m_height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = m_window.GetWindowNativeHandler();
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
        std::printf("Error: Failed to create D3D11 swap chain.\n");
        return false;
    }

    std::printf("D3D11 swap chain created.\n");
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
        std::printf("Error: Failed to create Render Target View for swap chain's buffer.\n");
        return false;
    }

    std::printf("Render Target View for swap chain's buffer created.\n");
    return true;
}

void Renderer::ClearColor(const mathfu::Vector4& color)
{
    m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), mathfu::Vector4Packed(color).data_);
}

void Renderer::Present()
{
    m_swapChain->Present(
        1, // VSync On
        0  // Flags
    );
}

bool Renderer::CreateShaders()
{
    std::string vertexShaderCode = ReadAssetFile("Shaders/VertexShader.hlsl");
    std::string pixelShaderCode = ReadAssetFile("Shaders/PixelShader.hlsl");

    return true;
}

void Renderer::DestroyShaders()
{
    m_vertexShaderBlob.Reset();
    m_vertexShader.Reset();
    m_pixelShader.Reset();
}

std::string Renderer::ReadAssetFile(const std::string& fileName) const
{
    auto fileNamePath = GetAssetPath() / fileName;
    if (!std::filesystem::exists(fileNamePath))
    {
        std::printf("Error: filename path %s does not exist.\n", fileNamePath.generic_string().c_str());
        return {};
    }

    if (std::ifstream file(fileNamePath);
        file.is_open())
    {
        std::ostringstream stringBuffer;
        stringBuffer << file.rdbuf();

        file.close();
        return stringBuffer.str();
    }
    else
    {
        std::printf("Error: filename path %s failed to open.\n", fileNamePath.generic_string().c_str());
        return {};
    }
}

std::filesystem::path Renderer::GetAssetPath() const
{
    auto execPath = GetExecutablePath();
    execPath /= "Assets";
    if (std::filesystem::exists(execPath))
    {
        return execPath;
    }

    // If the Assets folder is not in the same location as the executable,
    // that could be because the executable is being run from a build folder
    // (for example from Visual Studio). Try to find the 'build' folder to
    // extract the project path and use that to look for the Assets folder.
    if (auto it = execPath.generic_string().find("build");
        it != std::string::npos)
    {
        std::filesystem::path projectPath = execPath.generic_string().substr(0, it);
        projectPath /= "Assets";
        if (std::filesystem::exists(projectPath))
        {
            return projectPath;
        }
    }

    std::printf("Error: Assets path not found.\n");
    return {};
}

std::filesystem::path Renderer::GetExecutablePath() const
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    std::filesystem::path execPath(path);
    return execPath.remove_filename();
}
