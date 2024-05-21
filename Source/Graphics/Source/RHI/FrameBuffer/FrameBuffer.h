#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/FrameBuffer/FrameBufferDesc.h>
#include <Math/Color.h>

#include <optional>
#include <memory>

namespace DX
{
    class Device;
    class Texture;
    class RenderTargetView;
    class DepthStencilView;

    class FrameBuffer : public DeviceObject
    {
    public:
        FrameBuffer(Device* device, const FrameBufferDesc& desc);
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::FrameBuffer; }

        std::shared_ptr<RenderTargetView> GetColorRenderTargetView();
        std::shared_ptr<DepthStencilView> GetDepthStencilView();

    private:
        std::shared_ptr<Texture> m_colorTexture;
        std::shared_ptr<RenderTargetView> m_colorRenderTargetView;

        std::shared_ptr<Texture> m_depthStencilTexture;
        std::shared_ptr<DepthStencilView> m_depthStencilView;
    };
} // namespace DX
