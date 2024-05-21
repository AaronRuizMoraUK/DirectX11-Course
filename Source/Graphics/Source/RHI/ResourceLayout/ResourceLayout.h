#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/ResourceLayout/ResourceLayoutDesc.h>

namespace DX
{
    class ResourceLayout : public DeviceObject
    {
    public:
        ResourceLayout(Device* device, const ResourceLayoutDesc& desc);
        ~ResourceLayout() = default;

        ResourceLayout(const ResourceLayout&) = delete;
        ResourceLayout& operator=(const ResourceLayout&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::ResourceLayout; }
    };
} // namespace DX
