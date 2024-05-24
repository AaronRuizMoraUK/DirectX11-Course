#include <Renderer/Renderer.h>

#include <RHI/Device/Device.h>
#include <RHI/Device/DeviceContext.h>
#include <RHI/SwapChain/SwapChain.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Resource/Texture/Texture.h>

#include <Window/Window.h>
#include <Log/Log.h>

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

        DX_LOG(Info, "Renderer", "Initializing Renderer...");

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

        if (!CreateFrameBuffer())
        {
            Terminate();
            return false;
        }

        return true;
    }

    void Renderer::Terminate()
    {
        DX_LOG(Info, "Renderer", "Terminating Renderer...");

        m_frameBuffer.reset();
        m_swapChain.reset();
        m_device.reset();
    }

    Window* Renderer::GetWindow()
    {
        return m_window;
    }

    Device* Renderer::GetDevice()
    {
        return m_device.get();
    }

    bool Renderer::CreateDevice()
    {
        m_device = std::make_unique<Device>();

        if (!m_device)
        {
            DX_LOG(Error, "Renderer", "Failed to create device.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Device created.");
        return true;
    }

    bool Renderer::CreateSwapChain()
    {
        SwapChainDesc swapChainDesc = {};
        swapChainDesc.m_size = m_window->GetSize();
        swapChainDesc.m_refreshRate = m_window->GetRefreshRate();
        swapChainDesc.m_fullScreen = m_window->IsFullScreen();
        swapChainDesc.m_vSyncEnabled = m_window->IsVSyncEnabled();
        swapChainDesc.m_bufferCount = 1;
        swapChainDesc.m_bufferFormat = ResourceFormat::R8G8B8A8_UNORM;
        swapChainDesc.m_nativeWindowHandler = m_window->GetWindowNativeHandler();

        m_swapChain = m_device->CreateSwapChain(swapChainDesc);

        if (!m_swapChain)
        {
            DX_LOG(Error, "Renderer", "Failed to create swap chain.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Swap chain created.");
        return true;
    }

    bool Renderer::CreateFrameBuffer()
    {
        FrameBufferDesc frameBufferDesc = {};
        frameBufferDesc.m_renderTargetAttachments = FrameBufferDesc::TextureAttachments{ 
            {m_swapChain->GetBackBufferTexture(), m_swapChain->GetBackBufferTexture()->GetTextureDesc().m_format}
        };
        frameBufferDesc.m_createDepthStencilAttachment = true;

        m_frameBuffer = m_device->CreateFrameBuffer(frameBufferDesc);

        if (!m_swapChain)
        {
            DX_LOG(Error, "Renderer", "Failed to create frame buffer.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Swap chain created.");
        return true;
    }

    void Renderer::Clear(const Math::Color& color, float depth, uint8_t stencil)
    {
        m_device->GetImmediateContext().ClearFrameBuffer(*m_frameBuffer, color, depth, stencil);
    }

    void Renderer::Present()
    {
        m_swapChain->Present();
    }

    void Renderer::BindFramebuffer()
    {
        m_device->GetImmediateContext().BindFrameBuffer(*m_frameBuffer);
        m_device->GetImmediateContext().BindViewports({ Math::Rectangle{{0.0f, 0.0f}, Math::Vector2{m_window->GetSize()}} });
    }

    void Renderer::Draw(int indexCount)
    {
        m_device->GetImmediateContext().DrawIndexed(indexCount);
    }
} // namespace DX
