#include <Graphics/FrameBuffer/FrameBuffer.h>

namespace DX
{
    FrameBuffer::FrameBuffer(std::shared_ptr<Device> device, const FrameBufferDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
