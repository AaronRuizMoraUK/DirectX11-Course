#include <Graphics/CommandList/CommandList.h>

namespace DX
{
    CommandList::CommandList(Device* device, const CommandListDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
