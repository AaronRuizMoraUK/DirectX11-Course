#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Views/RenderTargetViewDesc.h>

#include <Math/Color.h>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11RenderTargetView;

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

        const RenderTargetViewDesc& GetRenderTargetViewDesc() const { return m_desc; }

        ComPtr<ID3D11RenderTargetView> GetDX11RenderTargetView();

    private:
        RenderTargetViewDesc m_desc;

    private:
        ComPtr<ID3D11RenderTargetView> m_dx11RenderTargetView;
    };
} // namespace DX
