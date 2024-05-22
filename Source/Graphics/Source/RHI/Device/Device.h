#pragma once

#include <vector>
#include <memory>

#include <RHI/DirectX/ComPtr.h>
class ID3D11Device;

namespace DX
{
    class DeviceObject;
    class DeviceContext;
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

    class Device
    {
    public:
        Device();
        ~Device();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

        // TODO: How to destroy device objects? Once created they are always kept inside m_deviceObjects.
        // TODO: Move initialization of all objects out of constructor

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
        std::shared_ptr<CommandList> CreateCommandList();

        void ExecuteCommandLists(std::vector<CommandList*> commandLists);

        DeviceContext& GetImmediateContext();

        ComPtr<ID3D11Device> GetDX11Device();

    private:
        using DeviceObjects = std::vector<std::shared_ptr<DeviceObject>>;

        DeviceObjects m_deviceObjects;

        std::unique_ptr<DeviceContext> m_immediateContext;

    private:
        ComPtr<ID3D11Device> m_dx11Device;
    };
} // namespace DX
