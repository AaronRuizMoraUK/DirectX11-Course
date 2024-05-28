#pragma once

#include <RHI/DeviceObject/DeviceObjectEnums.h>

namespace DX
{
    class Device;

    // Base class for all device objects.
    class DeviceObject
    {
    public:
        DeviceObject(Device* device);
        virtual ~DeviceObject() = default;

        DeviceObject(const DeviceObject&) = delete;
        DeviceObject& operator=(const DeviceObject&) = delete;

        virtual DeviceObjectType GetType() const = 0;

    protected:
        Device* m_ownerDevice = nullptr;
    };
} // namespace DX
