#include <RHI/CommandList/CommandList.h>

#include <RHI/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    CommandList::CommandList(Device* device)
        : DeviceContext(device, DeviceContextType::Deferred)
    {
        DX_LOG(Verbose, "CommandList", "Graphics command list created.");
    }

    CommandList::~CommandList()
    {
        DX_LOG(Verbose, "CommandList", "Graphics command list destroyed.");
    }

    void CommandList::Close()
    {
        const bool restoreDeferredContextState = false;

        // This will create the command list from the deferred context and record commands into it.
        // When the method returns, a command list is created containing all the render commands.
        auto result = GetDX11DeviceContext()->FinishCommandList(
            restoreDeferredContextState, m_dx11CommandList.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "CommandList", "Failed to created the command list from the deferred context.");
        }
    }

    void CommandList::Clear()
    {
        m_dx11CommandList.Reset();
    }

    ComPtr<ID3D11CommandList> CommandList::GetDX11CommandList()
    {
        return m_dx11CommandList;
    }
} // namespace DX
