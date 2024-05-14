#include <Graphics/CommandList/CommandList.h>

namespace DX
{
    CommandList::CommandList(std::shared_ptr<Device> device, const CommandListDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
