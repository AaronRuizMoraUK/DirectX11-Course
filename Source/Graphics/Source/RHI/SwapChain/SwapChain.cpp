#include <RHI/SwapChain/SwapChain.h>

#include <RHI/Device/Device.h>
#include <RHI/Resource/Texture/Texture.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <RHI/DirectX/Utils.h>

namespace DX
{
    SwapChain::SwapChain(Device* device, const SwapChainDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
        swapChainDesc.BufferDesc.Width = m_desc.m_size.x;
        swapChainDesc.BufferDesc.Height = m_desc.m_size.y;
        swapChainDesc.BufferDesc.RefreshRate.Numerator = m_desc.m_refreshRate;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.Format = DX::ToDX11ResourceFormat(m_desc.m_bufferFormat);
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = m_desc.m_bufferCount;
        swapChainDesc.OutputWindow = static_cast<HWND>(m_desc.m_nativeWindowHandler);
        swapChainDesc.Windowed = !m_desc.m_fullScreen;
        // Blt vs Flip:
        // - Blt will copy each back to front buffer. Slower.
        // - Flip will use 2+ bufferCount and no copy is required since it'll just alter which is the front and back buffer. Faster.
        // 
        // Discard vs Sequential
        // - Discard will disregard the buffer after presented. Good when entire frames are generated (for example, in games).
        // - Sequential will keep the buffer in case only portion of the buffer has changed. Good if only portions of the buffer change.
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
        // TODO: What happens when m_desc.m_bufferCount > 1
        // For example, when using DXGI_SWAP_EFFECT_FLIP_DISCARD, 2 buffers are required
        // and we need to alter on each frame which buffer to render to.
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
            backBufferTextureDesc.m_textureType = TextureType::Texture2D;
            backBufferTextureDesc.m_dimensions = Math::Vector3Int(dx11BackBufferDesc.Width, dx11BackBufferDesc.Height, 0);
            backBufferTextureDesc.m_mipCount = dx11BackBufferDesc.MipLevels;
            backBufferTextureDesc.m_format = m_desc.m_bufferFormat;
            backBufferTextureDesc.m_usage = ResourceUsage::Default;
            backBufferTextureDesc.m_bindFlags = TextureBind_RenderTarget;
            backBufferTextureDesc.m_cpuAccess = ResourceCPUAccess::None;
            backBufferTextureDesc.m_arrayCount = dx11BackBufferDesc.ArraySize;
            backBufferTextureDesc.m_sampleCount = dx11BackBufferDesc.SampleDesc.Count;
            backBufferTextureDesc.m_sampleQuality = dx11BackBufferDesc.SampleDesc.Quality;
            backBufferTextureDesc.m_initialDataIsNativeResource = true;
            backBufferTextureDesc.m_initialData = dx11BackBuffer.Get();

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
            m_desc.m_vSyncEnabled ? 1 : 0, // VSync
            0  // Flags
        );
    }

    std::shared_ptr<Texture> SwapChain::GetBackBufferTexture()
    {
        return m_backBufferTexture;
    }
} // namespace DX
