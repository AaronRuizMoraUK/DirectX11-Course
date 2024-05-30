#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/SwapChain/SwapChainDesc.h>

#include <memory>

#include <RHI/DirectX/ComPtr.h>
struct IDXGISwapChain;

namespace DX
{
    class Device;
    class Texture;
    class FrameBuffer;

    // A swap chain provides the mechanism to present the rendered image to the window.
    // A Frame Buffer is created from the swap chain internal buffer so it can be used
    // as the destination of the rendering.
    class SwapChain : public DeviceObject
    {
    public:
        SwapChain(Device* device, const SwapChainDesc& desc);
        ~SwapChain();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::SwapChain; }

        const SwapChainDesc& GetSwapChainDesc() const { return m_desc; }

        FrameBuffer* GetFrameBuffer();

        void Present();

        void OnResize(Math::Vector2Int size);

    private:
        bool CreateFrameBuffer();
        std::shared_ptr<Texture> CreateBackBufferTextureFromSwapChain();

        SwapChainDesc m_desc;

        // Swap chain creates and owns the main frame buffer.
        // This simplifies the process when having to flip or resize the swap chain.
        std::unique_ptr<FrameBuffer> m_frameBuffer;

    private:
        ComPtr<IDXGISwapChain> m_dx11SwapChain;
    };
} // namespace DX
