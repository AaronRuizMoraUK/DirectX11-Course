#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/FrameBuffer/FrameBufferDesc.h>

namespace DX
{
    class FrameBuffer : public DeviceObject
    {
    public:
        FrameBuffer(Device* device, const FrameBufferDesc& desc);
        ~FrameBuffer() = default;

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::FrameBuffer; }
    };
} // namespace DX
