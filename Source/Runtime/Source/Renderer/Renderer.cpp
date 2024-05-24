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

    FrameBuffer* Renderer::GetFrameBuffer()
    {
        return m_frameBuffer.get();
    }

    bool Renderer::CreateDevice()
    {
        m_device = std::make_unique<Device>();

        if (!m_device)
        {
            DX_LOG(Error, "Renderer", "Failed to create device.");
            return false;
        }

        return true;
    }

    bool Renderer::CreateSwapChain()
    {
        const uint32_t frameBufferCount = 2;

        SwapChainDesc swapChainDesc = {};
        swapChainDesc.m_size = m_window->GetSize();
        swapChainDesc.m_refreshRate = m_window->GetRefreshRate();
        swapChainDesc.m_fullScreen = m_window->IsFullScreen();
        swapChainDesc.m_vSyncEnabled = m_window->IsVSyncEnabled();
        swapChainDesc.m_bufferCount = frameBufferCount;
        swapChainDesc.m_bufferFormat = ResourceFormat::R8G8B8A8_UNORM;
        swapChainDesc.m_nativeWindowHandler = m_window->GetWindowNativeHandler();

        m_swapChain = m_device->CreateSwapChain(swapChainDesc);

        if (!m_swapChain)
        {
            DX_LOG(Error, "Renderer", "Failed to create swap chain.");
            return false;
        }

        return true;
    }

    bool Renderer::CreateFrameBuffer()
    {
        m_frameBuffer = m_swapChain->CreateFrameBuffer();

        if (!m_frameBuffer)
        {
            DX_LOG(Error, "Renderer", "Failed to create frame buffer.");
            return false;
        }

        return true;
    }

    void Renderer::Clear(const Math::Color& color, float depth, uint8_t stencil)
    {
        m_device->GetImmediateContext().ClearFrameBuffer(*m_frameBuffer, color, depth, stencil);
    }

    void Renderer::Present()
    {
        m_swapChain->Present(*m_frameBuffer);
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
