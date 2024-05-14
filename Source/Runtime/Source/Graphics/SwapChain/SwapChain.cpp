#include <Graphics/SwapChain/SwapChain.h>

namespace DX
{
    SwapChain::SwapChain(std::shared_ptr<Device> device, const SwapChainDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
