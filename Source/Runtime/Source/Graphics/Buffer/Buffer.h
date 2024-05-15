#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Buffer/BufferDesc.h>

namespace DX
{
    class Buffer : public DeviceObject
    {
    public:
        Buffer(Device* device, const BufferDesc& desc);
        ~Buffer() = default;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Buffer; }
    };
} // namespace DX
