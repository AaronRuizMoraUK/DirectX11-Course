#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Views/ShaderRWResourceViewDesc.h>

namespace DX
{
    class ShaderRWResourceView : public DeviceObject
    {
    public:
        ShaderRWResourceView(Device* device, const ShaderRWResourceViewDesc& desc);
        ~ShaderRWResourceView() = default;

        ShaderRWResourceView(const ShaderRWResourceView&) = delete;
        ShaderRWResourceView& operator=(const ShaderRWResourceView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::ShaderRWResourceView; }
    };
} // namespace DX
