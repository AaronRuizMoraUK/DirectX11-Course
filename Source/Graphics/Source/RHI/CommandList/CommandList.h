#pragma once

#include <RHI/Device/DeviceContext.h>

#include <memory>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11CommandList;

namespace DX
{
    // Command list to record graphics commands asynchronously.
    // Implemented to be similar to DirectX 12 command lists.
    // Internally it uses a DirectX 11 deferred device context.
    class CommandList : public DeviceContext
    {
    public:
        CommandList(Device* device);
        ~CommandList();

        CommandList(const CommandList&) = delete;
        CommandList& operator=(const CommandList&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::CommandList; }

        // -----------------------------------------------------------------------------
        // Call the following functions asynchronously from a thread to record commands.
        // -----------------------------------------------------------------------------
        // All functions from DeviceContext parent class.

        // Call this last after all the command calls.
        // Indicates that recording to the command list has finished.
        void Close();

        // -----------------------------------------------------------------------------
        // Called from the main thread to clear the command list after it's been
        // submitted for execution via Device::ExecuteCommandLists()
        // -----------------------------------------------------------------------------
        void Clear();

        ComPtr<ID3D11CommandList> GetDX11CommandList();

    private:
        ComPtr<ID3D11CommandList> m_dx11CommandList;
    };
} // namespace DX
