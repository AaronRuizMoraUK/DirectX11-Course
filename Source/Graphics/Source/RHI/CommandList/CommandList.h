#pragma once

#include <RHI/DeviceObject/DeviceObject.h>

#include <memory>

#include <RHI/DirectX/ComPtr.h>
class ID3D11CommandList;

namespace DX
{
    class DeviceContext;

    class CommandList : public DeviceObject
    {
    public:
        CommandList(Device* device);
        ~CommandList();

        CommandList(const CommandList&) = delete;
        CommandList& operator=(const CommandList&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::CommandList; }

        // --------------------------------------------------------------------
        // To be called asynchronously from a thread to executed commands
        // on the deferred context and obtain the command list.
        // --------------------------------------------------------------------
        DeviceContext& GetDeferredContext();

        // Call this last after all the command calls.
        // It creates the command list from the deferred context.
        void FinishCommandList();

        // --------------------------------------------------------------------
        // To be called from the main thread to obtain and clear the command
        // list.
        // --------------------------------------------------------------------
        void ClearCommandList();

        ComPtr<ID3D11CommandList> GetDX11CommandList();

    private:
        std::unique_ptr<DeviceContext> m_deferredContext;

    private:
        ComPtr<ID3D11CommandList> m_dx11CommandList;
    };
} // namespace DX
