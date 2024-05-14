#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/SwapChain/SwapChainDesc.h>

namespace DX
{
    class SwapChain : public DeviceObject
    {
    public:
        SwapChain(std::shared_ptr<Device> device, const SwapChainDesc& desc);
        ~SwapChain() = default;

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::SwapChain; }
    };
} // namespace DX
