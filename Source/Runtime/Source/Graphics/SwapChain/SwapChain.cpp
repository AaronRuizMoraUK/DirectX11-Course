#include <Graphics/SwapChain/SwapChain.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Window/Window.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

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
        swapChainDesc.BufferDesc.Format = DX::ToDX11ResourceFormat(desc.m_bufferFormat);
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

        // Create a texture with the back buffer of the swap chain.
        // TODO: What happens when desc.m_bufferCount > 1
        {
            // Get back buffer
            ComPtr<ID3D11Texture2D> dx11BackBuffer;
            result = m_dx11SwapChain->GetBuffer(0, IID_PPV_ARGS(dx11BackBuffer.GetAddressOf()));
            if (FAILED(result))
            {
                DX_LOG(Fatal, "SwapChain", "Failed to get back buffer from D3D11 swap chain.");
                return;
            }

            D3D11_TEXTURE2D_DESC dx11BackBufferDesc;
            dx11BackBuffer->GetDesc(&dx11BackBufferDesc);

            TextureDesc backBufferTextureDesc = {};
            backBufferTextureDesc.m_variant = TextureVariant::Texture2D;
            backBufferTextureDesc.m_size = mathfu::Vector3Int(dx11BackBufferDesc.Width, dx11BackBufferDesc.Height, 0);
            backBufferTextureDesc.m_mipLevels = dx11BackBufferDesc.MipLevels;
            backBufferTextureDesc.m_format = desc.m_bufferFormat;
            backBufferTextureDesc.m_usage = ResourceUsage::Default;
            backBufferTextureDesc.m_bindFlag = ResourceBind_RenderTarget;
            backBufferTextureDesc.m_cpuAccess = ResourceCPUAccess::None;
            backBufferTextureDesc.m_arraySize = dx11BackBufferDesc.ArraySize;
            backBufferTextureDesc.m_sampleCount = dx11BackBufferDesc.SampleDesc.Count;
            backBufferTextureDesc.m_sampleQuality = dx11BackBufferDesc.SampleDesc.Quality;
            backBufferTextureDesc.m_dataIsNativeResource = true;
            backBufferTextureDesc.m_data = dx11BackBuffer.Get();

            m_backBufferTexture = m_ownerDevice->CreateTexture(backBufferTextureDesc);
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

    std::shared_ptr<Texture> SwapChain::GetBackBufferTexture()
    {
        return m_backBufferTexture;
    }
} // namespace DX
