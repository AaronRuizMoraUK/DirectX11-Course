#pragma once

#include <Graphics/DeviceObject/DeviceObjectType.h>

#include <memory>

namespace DX
{
    class Device;

    class DeviceObject
    {
    public:
        DeviceObject(std::shared_ptr<Device> device);
        virtual ~DeviceObject() = default;

        DeviceObject(const DeviceObject&) = delete;
        DeviceObject& operator=(const DeviceObject&) = delete;

        virtual DeviceObjectType GetType() const = 0;

    protected:
        std::shared_ptr<Device> m_ownerDevice; // TODO: Does it need to be shared_ptr?
    };
} // namespace DX
