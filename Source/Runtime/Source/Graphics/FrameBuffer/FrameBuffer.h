#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/FrameBuffer/FrameBufferDesc.h>
#include <Math/Color.h>

#include <optional>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11Texture2D;
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

namespace DX
{
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
        ComPtr<ID3D11Texture2D> m_dx11ColorTexture;
        ComPtr<ID3D11RenderTargetView> m_dx11ColorRenderTargetView;

        ComPtr<ID3D11Texture2D> m_dx11DepthStencilTexture;
        ComPtr<ID3D11DepthStencilView> m_dx11DepthStencilView;
    };
} // namespace DX
