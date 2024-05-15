#include <Graphics/SwapChain/SwapChain.h>

#include <Graphics/Device/Device.h>
#include <Window/Window.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/TextureUtils.h>

namespace DX
{
    SwapChain::SwapChain(Device* device, const SwapChainDesc& desc)
        : DeviceObject(device)
        , m_vSyncEnabled(desc.m_window->IsVSyncEnabled())
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = desc.m_window->GetSize().x;
        swapChainDesc.BufferDesc.Height = desc.m_window->GetSize().y;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = desc.m_window->GetRefreshRate();
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = DX::ToDX11TextureFormat(desc.m_bufferFormat);
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = desc.m_bufferCount;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 1;
        swapChainDesc.OutputWindow = desc.m_window->GetWindowNativeHandler();
        swapChainDesc.Windowed = !desc.m_window->IsFullScreen();
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        ComPtr<IDXGIDevice> dxgiDevice;
        m_ownerDevice->GetDX11Device().As(&dxgiDevice);

        ComPtr<IDXGIAdapter> dxgiAdapter;
        dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());

        ComPtr<IDXGIFactory> dxgiFactory;
        dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

        auto result = dxgiFactory->CreateSwapChain(m_ownerDevice->GetDX11Device().Get(), &swapChainDesc, m_dx11SwapChain.GetAddressOf());
        if (FAILED(result))
        {
            DX_LOG(Fatal, "SwapChain", "Failed to create D3D11 swap chain.");
            return;
        }

        // Get back buffer
        result = m_dx11SwapChain->GetBuffer(0, IID_PPV_ARGS(m_dx11BackBuffer.GetAddressOf()));
        if (FAILED(result))
        {
            DX_LOG(Fatal, "SwapChain", "Failed to get back buffer from D3D11 swap chain.");
            return;
        }

        DX_LOG(Info, "SwapChain", "Graphics swap chain created.");
    }

    SwapChain::~SwapChain()
    {
        DX_LOG(Info, "SwapChain", "Graphics swap chain destroyed.");
    }

    void SwapChain::Present()
    {
        m_dx11SwapChain->Present(
            m_vSyncEnabled ? 1 : 0, // VSync
            0  // Flags
        );
    }

    ComPtr<ID3D11Texture2D> SwapChain::GetBackBuffer()
    {
        return m_dx11BackBuffer;
    }
} // namespace DX
