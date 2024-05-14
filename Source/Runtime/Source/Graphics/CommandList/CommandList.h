#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/CommandList/CommandListDesc.h>

namespace DX
{
    class CommandList : public DeviceObject
    {
    public:
        CommandList(std::shared_ptr<Device> device, const CommandListDesc& desc);
        ~CommandList() = default;

        CommandList(const CommandList&) = delete;
        CommandList& operator=(const CommandList&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::CommandList; }
    };
} // namespace DX
