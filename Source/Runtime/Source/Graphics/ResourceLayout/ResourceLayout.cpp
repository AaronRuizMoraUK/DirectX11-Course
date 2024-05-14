#include <Graphics/ResourceLayout/ResourceLayout.h>

namespace DX
{
    ResourceLayout::ResourceLayout(std::shared_ptr<Device> device, const ResourceLayoutDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
