#include <Graphics/Buffer/Buffer.h>

namespace DX
{
    Buffer::Buffer(std::shared_ptr<Device> device, const BufferDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
