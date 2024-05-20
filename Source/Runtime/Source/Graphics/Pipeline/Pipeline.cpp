#include <Graphics/Pipeline/Pipeline.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    Pipeline::Pipeline(Device* device, const PipelineDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        DX_LOG(Info, "Pipeline", "Graphics pipeline created.");
    }

    Pipeline::~Pipeline()
    {
        DX_LOG(Info, "Pipeline", "Graphics pipeline destroyed.");
    }
} // namespace DX
