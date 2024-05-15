#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Pipeline/PipelineDesc.h>

namespace DX
{
    class Pipeline : public DeviceObject
    {
    public:
        Pipeline(Device* device, const PipelineDesc& desc);
        ~Pipeline() = default;

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Pipeline; }
    };
} // namespace DX
