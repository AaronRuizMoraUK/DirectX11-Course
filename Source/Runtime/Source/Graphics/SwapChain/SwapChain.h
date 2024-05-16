#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/SwapChain/SwapChainDesc.h>

#include <memory>

#include <Graphics/DirectX/ComPtr.h>
class IDXGISwapChain;

namespace DX
{
    class Device;
    class Texture;

    class SwapChain : public DeviceObject
    {
    public:
        SwapChain(Device* device, const SwapChainDesc& desc);
        ~SwapChain();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::SwapChain; }

        void Present();

        std::shared_ptr<Texture> GetBackBufferTexture();

    private:
        bool m_vSyncEnabled = false;

        std::shared_ptr<Texture> m_backBufferTexture;

    private:
        ComPtr<IDXGISwapChain> m_dx11SwapChain;
    };
} // namespace DX
