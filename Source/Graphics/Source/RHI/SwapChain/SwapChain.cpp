#include <RHI/SwapChain/SwapChain.h>

#include <RHI/Device/Device.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Resource/Texture/Texture.h>
#include <Log/Log.h>
#include <Debug/Debug.h>

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
        swapChainDesc.SwapEffect = (m_desc.m_bufferCount > 1) ? DXGI_SWAP_EFFECT_FLIP_DISCARD : DXGI_SWAP_EFFECT_DISCARD;
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

        if (!CreateFrameBuffer())
        {
            DX_LOG(Fatal, "SwapChain", "Failed to create frame buffer for swap chain.");
            return;
        }

        DX_LOG(Info, "SwapChain", "Graphics swap chain created.");
    }

    SwapChain::~SwapChain()
    {
        // SwapChain will cause a crash when destroyed in full screen.
        // This is because the swap chain is still in use.
        if (m_dx11SwapChain && m_desc.m_fullScreen)
        {
            m_dx11SwapChain->SetFullscreenState(false, nullptr);
        }

        DX_LOG(Info, "SwapChain", "Graphics swap chain destroyed.");
    }

    FrameBuffer* SwapChain::GetFrameBuffer()
    {
        return m_frameBuffer.get();
    }

    void SwapChain::Present()
    {
        m_dx11SwapChain->Present(
            m_desc.m_vSyncEnabled ? 1 : 0, // VSync
            0  // Flags
        );

        if (m_desc.m_bufferCount > 1)
        {
            auto backBufferTexture = CreateBackBufferTextureFromSwapChain();

            m_frameBuffer->FlipSwapChainBackBuffer(backBufferTexture);
        }
    }

    void SwapChain::OnResize(Math::Vector2Int size)
    {
        // Release usage of back buffer texture by destroying the frame buffer
        m_frameBuffer.reset();

        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
            m_dx11SwapChain->GetDesc(&swapChainDesc);

            auto result = m_dx11SwapChain->ResizeBuffers(
                swapChainDesc.BufferCount,
                size.x,
                size.y,
                swapChainDesc.BufferDesc.Format,
                swapChainDesc.Flags
            );
            DX_ASSERT(SUCCEEDED(result), "SwapChain", "Failed to resize swap chain buffers.");
        }

        m_desc.m_size = size;

        // Re-create frame buffer
        CreateFrameBuffer();
    }

    bool SwapChain::CreateFrameBuffer()
    {
        auto backBufferTexture = CreateBackBufferTextureFromSwapChain();
        if (!backBufferTexture)
        {
            return false;
        }

        FrameBufferDesc frameBufferDesc = {};
        frameBufferDesc.m_renderTargetAttachments = FrameBufferDesc::TextureAttachments{
            {backBufferTexture, backBufferTexture->GetTextureDesc().m_format}
        };
        frameBufferDesc.m_createDepthStencilAttachment = true;

        // NOTE: Not created through owner device API to avoid having a
        // reference in the device as this is a sub-object of SwapChain.
        m_frameBuffer = std::make_unique<FrameBuffer>(m_ownerDevice, frameBufferDesc);

        return true;
    }

    std::shared_ptr<Texture> SwapChain::CreateBackBufferTextureFromSwapChain()
    {
        // Create texture with the first back buffer of the swap chain.

        // We only have access to the first back buffer of the swap chain that can be presented.
        ComPtr<ID3D11Texture2D> dx11BackBuffer;
        auto result = m_dx11SwapChain->GetBuffer(0, IID_PPV_ARGS(dx11BackBuffer.GetAddressOf()));
        if (FAILED(result))
        {
            DX_LOG(Fatal, "SwapChain", "Failed to get back buffer from D3D11 swap chain.");
            return {};
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

        // NOTE: Not created through owner device API to avoid having a
        // reference in the device as this is a sub-object of SwapChain.
        return std::make_shared<Texture>(m_ownerDevice, backBufferTextureDesc);
    }
} // namespace DX
