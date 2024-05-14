#include <Graphics/DeviceObject/DeviceObject.h>

namespace DX
{
    DeviceObject::DeviceObject(std::shared_ptr<Device> device)
    : m_ownerDevice(device)
    {
    }
} // namespace DX
