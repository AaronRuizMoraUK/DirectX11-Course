#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Views/DepthStencilViewDesc.h>

namespace DX
{
    class DepthStencilView : public DeviceObject
    {
    public:
        DepthStencilView(Device* device, const DepthStencilViewDesc& desc);
        ~DepthStencilView() = default;

        DepthStencilView(const DepthStencilView&) = delete;
        DepthStencilView& operator=(const DepthStencilView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::DepthStencilView; }
    };
} // namespace DX
