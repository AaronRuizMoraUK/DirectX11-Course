#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Views/RenderTargetViewDesc.h>

namespace DX
{
    class RenderTargetView : public DeviceObject
    {
    public:
        RenderTargetView(Device* device, const RenderTargetViewDesc& desc);
        ~RenderTargetView() = default;

        RenderTargetView(const RenderTargetView&) = delete;
        RenderTargetView& operator=(const RenderTargetView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::RenderTargetView; }
    };
} // namespace DX
