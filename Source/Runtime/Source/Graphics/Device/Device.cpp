#include <Graphics/Device/Device.h>

#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/Buffer/Buffer.h>
#include <Graphics/Texture/Texture.h>
#include <Graphics/Sampler/Sampler.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Pipeline/Pipeline.h>
#include <Graphics/ResourceLayout/ResourceLayout.h>
#include <Graphics/CommandList/CommandList.h>

#include <d3d11.h>

namespace DX
{
    Device::Device(const DeviceDesc& desc)
    {
    }

    std::shared_ptr<SwapChain> Device::CreateSwapChain(const SwapChainDesc& desc)
    {
        auto swapChain = std::make_shared<SwapChain>(shared_from_this(), desc);
        m_deviceObjects.push_back(swapChain);
        return swapChain;
    }

    std::shared_ptr<FrameBuffer> Device::CreateFrameBuffer(const FrameBufferDesc& desc)
    {
        auto frameBuffer = std::make_shared<FrameBuffer>(shared_from_this(), desc);
        m_deviceObjects.push_back(frameBuffer);
        return frameBuffer;
    }

    std::shared_ptr<Buffer> Device::CreateBuffer(const BufferDesc& desc)
    {
        auto buffer = std::make_shared<Buffer>(shared_from_this(), desc);
        m_deviceObjects.push_back(buffer);
        return buffer;
    }

    std::shared_ptr<Texture> Device::CreateTexture(const TextureDesc& desc)
    {
        auto texture = std::make_shared<Texture>(shared_from_this(), desc);
        m_deviceObjects.push_back(texture);
        return texture;
    }

    std::shared_ptr<Sampler> Device::CreateSampler(const SamplerDesc& desc)
    {
        auto sampler = std::make_shared<Sampler>(shared_from_this(), desc);
        m_deviceObjects.push_back(sampler);
        return sampler;
    }

    std::shared_ptr<Shader> Device::CreateShader(const ShaderDesc& desc)
    {
        auto shader = std::make_shared<Shader>(shared_from_this(), desc);
        m_deviceObjects.push_back(shader);
        return shader;
    }

    std::shared_ptr<Pipeline> Device::CreatePipeline(const PipelineDesc& desc)
    {
        auto pipeline = std::make_shared<Pipeline>(shared_from_this(), desc);
        m_deviceObjects.push_back(pipeline);
        return pipeline;
    }

    std::shared_ptr<ResourceLayout> Device::CreateResourceLayout(const ResourceLayoutDesc& desc)
    {
        auto resourceLayout = std::make_shared<ResourceLayout>(shared_from_this(), desc);
        m_deviceObjects.push_back(resourceLayout);
        return resourceLayout;
    }

    std::shared_ptr<CommandList> Device::CreateCommandList(const CommandListDesc& desc)
    {
        auto commandList = std::make_shared<CommandList>(shared_from_this(), desc);
        m_deviceObjects.push_back(commandList);
        return commandList;
    }
} // namespace DX
