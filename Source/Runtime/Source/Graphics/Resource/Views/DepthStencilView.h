#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Views/DepthStencilViewDesc.h>

#include <optional>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11DepthStencilView;

namespace DX
{
    class DepthStencilView : public DeviceObject
    {
    public:
        DepthStencilView(Device* device, const DepthStencilViewDesc& desc);
        ~DepthStencilView();

        DepthStencilView(const DepthStencilView&) = delete;
        DepthStencilView& operator=(const DepthStencilView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::DepthStencilView; }

        void Clear(std::optional<float> depth, std::optional<uint8_t> stencil);

        ComPtr<ID3D11DepthStencilView> GetDX11DepthStencilView();

    private:
        DepthStencilViewDesc m_desc;

    private:
        ComPtr<ID3D11DepthStencilView> m_dx11DepthStencilView;
    };
} // namespace DX
