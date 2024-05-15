#include <Graphics/DeviceObject/DeviceObject.h>

namespace DX
{
    DeviceObject::DeviceObject(Device* device)
    : m_ownerDevice(device)
    {
    }
} // namespace DX
