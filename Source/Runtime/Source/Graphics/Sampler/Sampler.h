#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Sampler/SamplerDesc.h>

namespace DX
{
    class Sampler : public DeviceObject
    {
    public:
        Sampler(Device* device, const SamplerDesc& desc);
        ~Sampler() = default;

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Sampler; }
    };
} // namespace DX
