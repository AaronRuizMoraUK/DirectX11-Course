#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/FrameBuffer/FrameBufferDesc.h>
#include <Math/Color.h>

#include <optional>
#include <memory>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

namespace DX
{
    class Device;
    class Texture;

    class FrameBuffer : public DeviceObject
    {
    public:
        FrameBuffer(Device* device, const FrameBufferDesc& desc);
        ~FrameBuffer();

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::FrameBuffer; }

        void Clear(std::optional<mathfu::Color> color, std::optional<float> depth, std::optional<uint8_t> stencil);

    private:
        std::shared_ptr<Texture> m_colorTexture;
        std::shared_ptr<Texture> m_depthStencilTexture;

    private:
        ComPtr<ID3D11RenderTargetView> m_dx11ColorRenderTargetView;
        ComPtr<ID3D11DepthStencilView> m_dx11DepthStencilView;
    };
} // namespace DX
