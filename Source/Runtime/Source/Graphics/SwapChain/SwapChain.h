#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/SwapChain/SwapChainDesc.h>

#include <Graphics/DirectX/ComPtr.h>
class IDXGISwapChain;
class ID3D11Texture2D;

namespace DX
{
    class SwapChain : public DeviceObject
    {
    public:
        SwapChain(Device* device, const SwapChainDesc& desc);
        ~SwapChain();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::SwapChain; }

        void Present();

        ComPtr<ID3D11Texture2D> GetBackBuffer();

    private:
        bool m_vSyncEnabled = false;

    private:
        ComPtr<IDXGISwapChain> m_dx11SwapChain;
        ComPtr<ID3D11Texture2D> m_dx11BackBuffer;
    };
} // namespace DX
