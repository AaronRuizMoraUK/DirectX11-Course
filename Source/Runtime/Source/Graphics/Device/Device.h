#pragma once

#include <Graphics/Device/DeviceDesc.h>

#include <vector>
#include <memory>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11Device;
class ID3D11DeviceContext;

namespace DX
{
    class DeviceObject;
    class SwapChain;
    class FrameBuffer;
    class Buffer;
    class Texture;
    class Sampler;
    class Shader;
    class ShaderResourceView;
    class ShaderRWResourceView;
    class RenderTargetView;
    class DepthStencilView;
    class Pipeline;
    class ResourceLayout;
    class CommandList;

    struct SwapChainDesc;
    struct FrameBufferDesc;
    struct BufferDesc;
    struct TextureDesc;
    struct SamplerDesc;
    struct ShaderDesc;
    struct ShaderResourceViewDesc;
    struct ShaderRWResourceViewDesc;
    struct RenderTargetViewDesc;
    struct DepthStencilViewDesc;
    struct PipelineDesc;
    struct ResourceLayoutDesc;
    struct CommandListDesc;

    namespace Internal
    {
        template<typename Tag>
        using DeviceIdType = uint32_t;
    }
    using DeviceId = Internal::DeviceIdType<struct DeviceIdTag>;
    static const DeviceId InvalidDeviceId = 0xFFFFFFFF;

    class Device
    {
    public:
        Device(DeviceId deviceId, const DeviceDesc& desc);
        ~Device();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

        DeviceId GetId() const { return m_deviceId; }

        std::shared_ptr<SwapChain> CreateSwapChain(const SwapChainDesc& desc);
        std::shared_ptr<FrameBuffer> CreateFrameBuffer(const FrameBufferDesc& desc);
        std::shared_ptr<Buffer> CreateBuffer(const BufferDesc& desc);
        std::shared_ptr<Texture> CreateTexture(const TextureDesc& desc);
        std::shared_ptr<Sampler> CreateSampler(const SamplerDesc& desc);
        std::shared_ptr<Shader> CreateShader(const ShaderDesc& desc);
        std::shared_ptr<ShaderResourceView> CreateShaderResourceView(const ShaderResourceViewDesc& desc);
        std::shared_ptr<ShaderRWResourceView> CreateShaderRWResourceView(const ShaderRWResourceViewDesc& desc);
        std::shared_ptr<RenderTargetView> CreateRenderTargetView(const RenderTargetViewDesc& desc);
        std::shared_ptr<DepthStencilView> CreateDepthStencilView(const DepthStencilViewDesc& desc);
        std::shared_ptr<Pipeline> CreatePipeline(const PipelineDesc& desc);
        std::shared_ptr<ResourceLayout> CreateResourceLayout(const ResourceLayoutDesc& desc);
        std::shared_ptr<CommandList> CreateCommandList(const CommandListDesc& desc);

        //std::shared_ptr<SwapChain> GetSwapChain() { return m_swapChain; }

        ComPtr<ID3D11Device> GetDX11Device();
        ComPtr<ID3D11DeviceContext> GetDX11ImmediateContext();

    private:
        using DeviceObjects = std::vector<std::shared_ptr<DeviceObject>>;

        const DeviceId m_deviceId = InvalidDeviceId;
        DeviceObjects m_deviceObjects;
        //std::shared_ptr<SwapChain> m_swapChain;

    private:
        ComPtr<ID3D11Device> m_dx11Device;
        ComPtr<ID3D11DeviceContext> m_dx11ImmediateContext;
    };
} // namespace DX
