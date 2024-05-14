#include <Graphics/Pipeline/Pipeline.h>

namespace DX
{
    Pipeline::Pipeline(std::shared_ptr<Device> device, const PipelineDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
