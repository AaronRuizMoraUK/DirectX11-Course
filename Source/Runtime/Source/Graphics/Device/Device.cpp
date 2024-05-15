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

#include <Log/Log.h>

#include <array>
#include <numeric>

#include <d3d11.h>

namespace DX
{
    Device::Device(const DeviceDesc& desc)
    {
        const std::array<D3D_FEATURE_LEVEL, 1> featureLevels = { D3D_FEATURE_LEVEL_11_1 };

        auto result = D3D11CreateDevice(
            nullptr, // IDXIGAdapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // Software
            0, // Flags
            featureLevels.data(),
            featureLevels.size(),
            D3D11_SDK_VERSION,
            m_dx11Device.GetAddressOf(),
            nullptr, // Feature level selected
            m_dx11ImmediateContext.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Fatal, "Device", "Failed to create D3D11 device.");
            return;
        }

        DX_LOG(Info, "Device", "Graphics device created.");
    }

    Device::~Device()
    {
        int leakCount = std::reduce(m_deviceObjects.begin(), m_deviceObjects.end(), 0,
            [](int count, const auto& deviceObject)
            {
                return count + (deviceObject.use_count() > 1) ? 1 : 0;
            });
        if (leakCount > 0)
        {
            DX_LOG(Warning, "Device", "There are %d graphics objects that are still referenced when destroying the graphics device.", leakCount);
        }

        m_deviceObjects.clear();

        DX_LOG(Info, "Device", "Graphics device destroyed.");
    }

    std::shared_ptr<SwapChain> Device::CreateSwapChain(const SwapChainDesc& desc)
    {
        auto swapChain = std::make_shared<SwapChain>(this, desc);
        m_deviceObjects.push_back(swapChain);
        return swapChain;
    }

    std::shared_ptr<FrameBuffer> Device::CreateFrameBuffer(const FrameBufferDesc& desc)
    {
        auto frameBuffer = std::make_shared<FrameBuffer>(this, desc);
        m_deviceObjects.push_back(frameBuffer);
        return frameBuffer;
    }

    std::shared_ptr<Buffer> Device::CreateBuffer(const BufferDesc& desc)
    {
        auto buffer = std::make_shared<Buffer>(this, desc);
        m_deviceObjects.push_back(buffer);
        return buffer;
    }

    std::shared_ptr<Texture> Device::CreateTexture(const TextureDesc& desc)
    {
        auto texture = std::make_shared<Texture>(this, desc);
        m_deviceObjects.push_back(texture);
        return texture;
    }

    std::shared_ptr<Sampler> Device::CreateSampler(const SamplerDesc& desc)
    {
        auto sampler = std::make_shared<Sampler>(this, desc);
        m_deviceObjects.push_back(sampler);
        return sampler;
    }

    std::shared_ptr<Shader> Device::CreateShader(const ShaderDesc& desc)
    {
        auto shader = std::make_shared<Shader>(this, desc);
        m_deviceObjects.push_back(shader);
        return shader;
    }

    std::shared_ptr<Pipeline> Device::CreatePipeline(const PipelineDesc& desc)
    {
        auto pipeline = std::make_shared<Pipeline>(this, desc);
        m_deviceObjects.push_back(pipeline);
        return pipeline;
    }

    std::shared_ptr<ResourceLayout> Device::CreateResourceLayout(const ResourceLayoutDesc& desc)
    {
        auto resourceLayout = std::make_shared<ResourceLayout>(this, desc);
        m_deviceObjects.push_back(resourceLayout);
        return resourceLayout;
    }

    std::shared_ptr<CommandList> Device::CreateCommandList(const CommandListDesc& desc)
    {
        auto commandList = std::make_shared<CommandList>(this, desc);
        m_deviceObjects.push_back(commandList);
        return commandList;
    }

    ComPtr<ID3D11Device> Device::GetDX11Device()
    {
        return m_dx11Device;
    }

    ComPtr<ID3D11DeviceContext> Device::GetDX11ImmediateContext()
    {
        return m_dx11ImmediateContext;
    }
} // namespace DX
