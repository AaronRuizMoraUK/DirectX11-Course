#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Texture/TextureDesc.h>

namespace DX
{
    class Texture : public DeviceObject
    {
    public:
        Texture(std::shared_ptr<Device> device, const TextureDesc& desc);
        ~Texture() = default;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Texture; }
    };
} // namespace DX
