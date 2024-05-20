#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>

#include <Math/Rectangle.h>
#include <Math/Color.h>
#include <vector>
#include <optional>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11DeviceContext;

namespace DX
{
    class Buffer;
    class FrameBuffer;
    class Pipeline;

    enum DeviceContextType
    {
        Unknown = 0,

        Immediate,
        Deferred,

        Count
    };

    class DeviceContext : public DeviceObject
    {
    public:
        DeviceContext(Device* device, DeviceContextType type, void* nativeContext = nullptr);
        ~DeviceContext();

        DeviceContext(const DeviceContext&) = delete;
        DeviceContext& operator=(const DeviceContext&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::DeviceContext; }

        void BindFrameBuffer(FrameBuffer& frameBuffer);
        void BindPipeline(Pipeline& pipeline);

        // Which viewport/scissor to use is determined by the SV_ViewportArrayIndex semantic output by a geometry shader.
        // When not specified it'll use the first one.
        void BindViewports(const std::vector<Math::Rectangle>& rectangles);
        void BindScissors(const std::vector<Math::RectangleInt>& rectangles); // Only used when scissors enabled in pipeline's rasterizer state

        void BindVertexBuffers(const std::vector<Buffer*>& vertexBuffers);
        void BindIndexBuffer(Buffer& indexBuffer);
        void BindResources(); // CB, SRV, SRWRV, Samplers. Needs resource layouts filled for each shader.

        void ClearFrameBuffer(FrameBuffer& frameBuffer,
            std::optional<Math::Color> color,
            std::optional<float> depth = std::nullopt,
            std::optional<uint8_t> stencil = std::nullopt);

        void DrawIndexed(uint32_t indexCount, uint32_t indexOffset = 0, uint32_t vertexOffset = 0);

        void UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize);

        ComPtr<ID3D11DeviceContext> GetDX11DeviceContext();

    private:
        DeviceContextType m_type;

    private:
        ComPtr<ID3D11DeviceContext> m_dx11DeviceContext;
    };
} // namespace DX
