#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/FrameBuffer/FrameBufferDesc.h>
#include <Math/Color.h>

#include <optional>
#include <memory>
#include <vector>

namespace DX
{
    class Device;
    class Texture;
    class RenderTargetView;
    class DepthStencilView;

    // Collection of render target views and depth stencil view.
    // A frame buffer is bound with device context to be the
    // destination of all rendering operations.
    class FrameBuffer : public DeviceObject
    {
    public:
        FrameBuffer(Device* device, const FrameBufferDesc& desc);
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::FrameBuffer; }

        const FrameBufferDesc& GetDesc() const { return m_desc; }

        const std::vector<std::shared_ptr<RenderTargetView>>& GetRenderTargetViews();
        std::shared_ptr<DepthStencilView> GetDepthStencilView();

    protected:
        friend class SwapChain;
        void FlipSwapChainBackBuffer(std::shared_ptr<Texture> backBuffer);

    private:
        FrameBufferDesc m_desc;

        std::vector<std::shared_ptr<RenderTargetView>> m_renderTargetViews;
        std::shared_ptr<DepthStencilView> m_depthStencilView;
    };
} // namespace DX
