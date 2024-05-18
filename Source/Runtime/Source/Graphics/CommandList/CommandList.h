#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>

#include <Math/Vector2.h>
#include <Math/Color.h>
#include <optional>

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

        void BindFrameBuffer(FrameBuffer& frameBuffer);
        void BindPipeline(Pipeline& pipeline);
        void BindViewport(const mathfu::Vector2& topLeft, const mathfu::Vector2& size);
        //void BindScissor(const mathfu::Vector2Int& startPos, const mathfu::Vector2Int& size); // TODO

        void BindVertexBuffer(Buffer& vertexBuffer);
        void BindIndexBuffer(Buffer& indexBuffer);
        void BindResources(); // CB, SRV, SRWRV, Samplers. Needs resource layouts filled for each shader.

        void ClearFrameBuffer(FrameBuffer& frameBuffer, 
            std::optional<mathfu::Color> color,
            std::optional<float> depth,
            std::optional<uint8_t> stencil);

        void DrawIndexed(uint32_t indexCount);

        void UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize);

        void ClearCommandList();

    private:
        ComPtr<ID3D11CommandList> m_dx11CommandList;
        ComPtr<ID3D11DeviceContext> m_dx11DeferredContext;
    };
} // namespace DX
