#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Views/RenderTargetViewDesc.h>

#include <Math/Color.h>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11RenderTargetView;

namespace DX
{
    class RenderTargetView : public DeviceObject
    {
    public:
        RenderTargetView(Device* device, const RenderTargetViewDesc& desc);
        ~RenderTargetView();

        RenderTargetView(const RenderTargetView&) = delete;
        RenderTargetView& operator=(const RenderTargetView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::RenderTargetView; }

        ComPtr<ID3D11RenderTargetView> GetDX11RenderTargetView();

    private:
        RenderTargetViewDesc m_desc;

    private:
        ComPtr<ID3D11RenderTargetView> m_dx11RenderTargetView;
    };
} // namespace DX
