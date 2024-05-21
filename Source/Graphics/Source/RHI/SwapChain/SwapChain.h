#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/SwapChain/SwapChainDesc.h>

#include <memory>

#include <RHI/DirectX/ComPtr.h>
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

        const SwapChainDesc& GetSwapChainDesc() const { return m_desc; }

        void Present();

        std::shared_ptr<Texture> GetBackBufferTexture();

    private:
        SwapChainDesc m_desc;

        std::shared_ptr<Texture> m_backBufferTexture;

    private:
        ComPtr<IDXGISwapChain> m_dx11SwapChain;
    };
} // namespace DX
