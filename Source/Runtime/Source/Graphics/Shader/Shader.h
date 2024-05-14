#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Shader/ShaderDesc.h>

namespace DX
{
    class Shader : public DeviceObject
    {
    public:
        Shader(std::shared_ptr<Device> device, const ShaderDesc& desc);
        ~Shader() = default;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Shader; }
    };
} // namespace DX
