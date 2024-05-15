#pragma once

#include <Graphics/Device/Device.h>

#include <memory>
#include <unordered_map>

namespace DX
{
    class DeviceManager
    {
    public:
        static DeviceManager& Get();
        static void Destroy();

        ~DeviceManager();

        // Delete copy constructor and assignment operator to prevent copying
        DeviceManager(const DeviceManager&) = delete;
        DeviceManager& operator=(const DeviceManager&) = delete;

        Device* CreateDevice(const DeviceDesc& desc);
        void DestroyDevice(DeviceId rendererId);

        Device* GetDevice(DeviceId rendererId);

    private:
        DeviceManager();

        static std::unique_ptr<DeviceManager> Instance;
        static DeviceId NextDeviceId;

        using Devices = std::unordered_map<DeviceId, std::unique_ptr<Device>>;
        Devices m_devices;
    };
} // namespace DX
