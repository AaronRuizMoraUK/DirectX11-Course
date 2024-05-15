#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Views/ShaderResourceViewDesc.h>

namespace DX
{
    class ShaderResourceView : public DeviceObject
    {
    public:
        ShaderResourceView(Device* device, const ShaderResourceViewDesc& desc);
        ~ShaderResourceView() = default;

        ShaderResourceView(const ShaderResourceView&) = delete;
        ShaderResourceView& operator=(const ShaderResourceView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::ShaderResourceView; }
    };
} // namespace DX
