#include <Graphics/Sampler/Sampler.h>

namespace DX
{
    Sampler::Sampler(std::shared_ptr<Device> device, const SamplerDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
