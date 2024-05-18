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

    void CommandList::BindVertexBuffer(Buffer& vertexBuffer)
    {
        const uint32_t vertexBufferStride = vertexBuffer.GetBufferDesc().m_elementSizeInBytes;
        const uint32_t vertexBufferOffset = 0;

        m_dx11DeferredContext->IASetVertexBuffers(0, 1, vertexBuffer.GetDX11Buffer().GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
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
        // Bad idea as it has to add the command to the command list.
        frameBuffer.Clear(color, depth, stencil); // TODO: remove clear function in frame buffer and views.
    }

    void CommandList::DrawIndexed(uint32_t indexCount)
    {
    }

    void CommandList::UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize)
    {
    }

    void CommandList::ClearCommandList()
    {
    }
} // namespace DX
