#include <Graphics/CommandList/CommandList.h>

#include <Graphics/Device/Device.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/Resource/Views/RenderTargetView.h>
#include <Graphics/Resource/Views/DepthStencilView.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    CommandList::CommandList(Device* device)
        : DeviceObject(device)
    {
        auto result = m_ownerDevice->GetDX11Device()->CreateDeferredContext(0, m_dx11DeferredContext.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Fatal, "CommandList", "Failed to command list.");
            return;
        }

        DX_LOG(Verbose, "CommandList", "Graphics command list created.");
    }

    CommandList::~CommandList()
    {
        DX_LOG(Verbose, "CommandList", "Graphics command list destroyed.");
    }

    void CommandList::BindFrameBuffer(FrameBuffer& frameBuffer)
    {
        m_dx11DeferredContext->OMSetRenderTargets(1, 
            frameBuffer.GetColorRenderTargetView()->GetDX11RenderTargetView().GetAddressOf(),
            frameBuffer.GetDepthStencilView()->GetDX11DepthStencilView().Get());
    }

    void CommandList::BindPipeline(Pipeline& pipeline)
    {
    }

    void CommandList::BindViewport(const mathfu::Vector2& topLeft, const mathfu::Vector2& size)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = topLeft.x;
        viewport.TopLeftY = topLeft.y;
        viewport.Width = size.x;
        viewport.Height = size.y;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_dx11DeferredContext->RSSetViewports(1, &viewport);
    }

    void CommandList::BindVertexBuffers(const std::vector<Buffer*>& vertexBuffers)
    {
        for (int i = 0; i < vertexBuffers.size(); ++i)
        {
            const uint32_t vertexBufferStride = vertexBuffers[i]->GetBufferDesc().m_elementSizeInBytes;
            const uint32_t vertexBufferOffset = 0;

            m_dx11DeferredContext->IASetVertexBuffers(
                i, 1, vertexBuffers[i]->GetDX11Buffer().GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
        }
    }

    void CommandList::BindIndexBuffer(Buffer& indexBuffer)
    {
        ResourceFormat indexFormat;
        switch (indexBuffer.GetBufferDesc().m_elementSizeInBytes)
        {
        case 2:
            indexFormat = ResourceFormat::R16_UINT;
            break;
        case 4:
            indexFormat = ResourceFormat::R32_UINT;
            break;
        default:
            DX_LOG(Fatal, "CommandList", "Index buffer format not supported.");
            return;
        }

        m_dx11DeferredContext->IASetIndexBuffer(indexBuffer.GetDX11Buffer().Get(), ToDX11ResourceFormat(indexFormat), 0);
    }

    void CommandList::BindResources()
    {
    }

    void CommandList::ClearFrameBuffer(FrameBuffer& frameBuffer,
        std::optional<mathfu::Color> color,
        std::optional<float> depth,
        std::optional<uint8_t> stencil)
    {
        if (auto rtv = frameBuffer.GetColorRenderTargetView();
            rtv&& color.has_value())
        {
            m_dx11DeferredContext->ClearRenderTargetView(
                rtv->GetDX11RenderTargetView().Get(), mathfu::ColorPacked(*color).data_);
        }

        if (auto dsv = frameBuffer.GetDepthStencilView())
        {
            if (depth.has_value() || stencil.has_value())
            {
                uint32_t flags = (depth.has_value() ? D3D11_CLEAR_DEPTH : 0) | (stencil.has_value() ? D3D11_CLEAR_STENCIL : 0);

                m_dx11DeferredContext->ClearDepthStencilView(
                    dsv->GetDX11DepthStencilView().Get(), flags, depth.value_or(1.0f), stencil.value_or(0));
            }
        }
    }

    void CommandList::DrawIndexed(uint32_t indexCount, uint32_t indexOffset, uint32_t vertexOffset)
    {
        m_dx11DeferredContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
    }

    void CommandList::UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        m_dx11DeferredContext->Map(buffer.GetDX11Buffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, data, dataSize);
        m_dx11DeferredContext->Unmap(buffer.GetDX11Buffer().Get(), 0);
    }

    void CommandList::FinishCommandList()
    {
        const bool restoreDeferredContextState = false;

        // This will create the command list from the deferred context and record commands into it.
        // When the method returns, a command list is created containing all the render commands.
        auto result = m_dx11DeferredContext->FinishCommandList(
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

    ComPtr<ID3D11DeviceContext> CommandList::GetDX11DeferredContext()
    {
        return m_dx11DeferredContext;
    }
} // namespace DX
