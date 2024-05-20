#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>

#include <Math/Rectangle.h>
#include <Math/Color.h>
#include <optional>
#include <vector>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11CommandList;
class ID3D11DeviceContext;

namespace DX
{
    class FrameBuffer;
    class Pipeline;
    class Buffer;

    class CommandList : public DeviceObject
    {
    public:
        CommandList(Device* device);
        ~CommandList();

        CommandList(const CommandList&) = delete;
        CommandList& operator=(const CommandList&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::CommandList; }

        // --------------------------------------------------------------------
        // Commands. To be called asynchronously from a thread.
        // --------------------------------------------------------------------
        void BindFrameBuffer(FrameBuffer& frameBuffer);
        void BindPipeline(Pipeline& pipeline);

        // Which viewport/scissor to use is determined by the SV_ViewportArrayIndex semantic output by a geometry shader.
        // When not specified it'll use the first one.
        void BindViewports(const std::vector<mathfu::Rectangle>& rectangles);
        void BindScissors(const std::vector<mathfu::RectangleInt>& rectangles); // Only used when scissors enabled in pipeline's rasterizer state

        void BindVertexBuffers(const std::vector<Buffer*>& vertexBuffers);
        void BindIndexBuffer(Buffer& indexBuffer);
        void BindResources(); // CB, SRV, SRWRV, Samplers. Needs resource layouts filled for each shader.

        void ClearFrameBuffer(FrameBuffer& frameBuffer, 
            std::optional<mathfu::Color> color,
            std::optional<float> depth = std::nullopt,
            std::optional<uint8_t> stencil = std::nullopt);

        void DrawIndexed(uint32_t indexCount, uint32_t indexOffset = 0, uint32_t vertexOffset = 0);

        void UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize);

        // Call this last after all the command calls.
        // It creates the command list from the deferred context.
        void FinishCommandList();

        // --------------------------------------------------------------------
        // Command list clear. To be called from the main thread.
        // --------------------------------------------------------------------
        void ClearCommandList();

        ComPtr<ID3D11CommandList> GetDX11CommandList();
        ComPtr<ID3D11DeviceContext> GetDX11DeferredContext();

    private:
        ComPtr<ID3D11CommandList> m_dx11CommandList;
        ComPtr<ID3D11DeviceContext> m_dx11DeferredContext;
    };
} // namespace DX
