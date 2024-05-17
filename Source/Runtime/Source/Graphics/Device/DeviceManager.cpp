#include <Graphics/Device/DeviceManager.h>

#include <Log/Log.h>

namespace DX
{
    std::unique_ptr<DeviceManager> DeviceManager::Instance;

    DeviceId DeviceManager::NextDeviceId = DefaultDeviceId;

    DeviceManager& DeviceManager::Get()
    {
        if (!Instance)
        {
            Instance.reset(new DeviceManager());
        }
        return *Instance;
    }

    void DeviceManager::Destroy()
    {
        Instance.reset();
    }

    DeviceManager::DeviceManager()
    {
        DX_LOG(Info, "Device Manager", "Initializing graphics device manager...");
    }

    DeviceManager::~DeviceManager()
    {
        m_devices.clear();

        DX_LOG(Info, "Device Manager", "Terminating graphics device manager...");
    }

    Device* DeviceManager::CreateDevice(const DeviceDesc& desc)
    {
        auto newDevice = std::make_unique<Device>(NextDeviceId, desc);
        auto result = m_devices.emplace(NextDeviceId, std::move(newDevice));

        NextDeviceId = DeviceId(NextDeviceId.GetValue() + 1);

        // Result's first is the iterator to the newly inserted element (pair),
        // its second is the value of the element (unique_ptr<Device>).
        return result.first->second.get();
    }

    void DeviceManager::DestroyDevice(DeviceId deviceId)
    {
        if (deviceId != DefaultDeviceId)
        {
            // Removing the devices from the map will destroy it.
            m_devices.erase(deviceId);
        }
    }

    Device* DeviceManager::GetDevice(DeviceId deviceId)
    {
        if (auto it = m_devices.find(deviceId);
            it != m_devices.end())
        {
            return it->second.get();
        }
        return nullptr;
    }
} // namespace DX
