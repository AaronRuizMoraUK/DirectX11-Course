#include <Graphics/CommandList/CommandList.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Device/DeviceContext.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    CommandList::CommandList(Device* device)
        : DeviceObject(device)
    {
        // Create the deferred context.
        m_deferredContext = std::make_unique<DeviceContext>(device, DeviceContextType::Deferred);
        if (!m_deferredContext)
        {
            DX_LOG(Fatal, "CommandList", "Failed to create deferred device context.");
            return;
        }

        DX_LOG(Verbose, "CommandList", "Graphics command list created.");
    }

    CommandList::~CommandList()
    {
        DX_LOG(Verbose, "CommandList", "Graphics command list destroyed.");
    }

    DeviceContext& CommandList::GetDeferredContext()
    {
        return *m_deferredContext;
    }

    void CommandList::FinishCommandList()
    {
        const bool restoreDeferredContextState = false;

        // This will create the command list from the deferred context and record commands into it.
        // When the method returns, a command list is created containing all the render commands.
        auto result = m_deferredContext->GetDX11DeviceContext()->FinishCommandList(
            restoreDeferredContextState, m_dx11CommandList.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "CommandList", "Failed to created the command list from the deferred context.");
        }
    }

    void CommandList::ClearCommandList()
    {
        m_dx11CommandList.Reset();
    }

    ComPtr<ID3D11CommandList> CommandList::GetDX11CommandList()
    {
        return m_dx11CommandList;
    }
} // namespace DX
