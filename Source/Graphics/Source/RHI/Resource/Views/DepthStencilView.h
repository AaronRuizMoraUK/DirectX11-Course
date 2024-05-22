#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Views/DepthStencilViewDesc.h>

#include <optional>

#include <RHI/DirectX/ComPtr.h>
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

        const DepthStencilViewDesc& GetDepthStencilViewDesc() const { return m_desc; }

        ComPtr<ID3D11DepthStencilView> GetDX11DepthStencilView();

    private:
        DepthStencilViewDesc m_desc;

    private:
        ComPtr<ID3D11DepthStencilView> m_dx11DepthStencilView;
    };
} // namespace DX
