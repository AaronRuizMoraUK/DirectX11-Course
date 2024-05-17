#include <Graphics/Device/Device.h>

#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Sampler/Sampler.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Resource/Views/ShaderResourceView.h>
#include <Graphics/Resource/Views/ShaderRWResourceView.h>
#include <Graphics/Resource/Views/RenderTargetView.h>
#include <Graphics/Resource/Views/DepthStencilView.h>
#include <Graphics/Pipeline/Pipeline.h>
#include <Graphics/ResourceLayout/ResourceLayout.h>
#include <Graphics/CommandList/CommandList.h>

#include <Log/Log.h>

#include <array>
#include <algorithm>

#include <d3d11.h>

namespace DX
{
    Device::Device(DeviceId deviceId, const DeviceDesc& desc)
        : m_deviceId(deviceId)
    {
        const std::array<D3D_FEATURE_LEVEL, 1> featureLevels = { D3D_FEATURE_LEVEL_11_1 };

        uint32_t flags = 0;
#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        auto result = D3D11CreateDevice(
            nullptr, // IDXIGAdapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // Software
            flags,
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

        DX_LOG(Info, "Device", "Graphics device %u created.", m_deviceId);
    }

    Device::~Device()
    {
#ifndef NDEBUG
        // Erase iteratively device objects that have no external references (its count is 1).
        // Stop when all objects are erased or when no more objects can be erased.
        // If there are still objects in the list, that means they are still referenced somewhere
        // and could be a memory leak.
        {
            int objectsCount;
            do
            {
                objectsCount = m_deviceObjects.size();
                m_deviceObjects.erase(
                    std::remove_if(m_deviceObjects.begin(), m_deviceObjects.end(), [](const auto& deviceObject)
                        {
                            return deviceObject.use_count() == 1;
                        }),
                    m_deviceObjects.end());
            } while (objectsCount != m_deviceObjects.size() && !m_deviceObjects.empty());

            if (!m_deviceObjects.empty())
            {
                DX_LOG(Warning, "Device", "There are %d device objects still referenced at the time of destroying device %u.", m_deviceObjects.size(), m_deviceId);
            }
        }
#endif

        m_deviceObjects.clear();

        DX_LOG(Info, "Device", "Graphics device %u destroyed.", m_deviceId);
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

    std::shared_ptr<ShaderResourceView> Device::CreateShaderResourceView(const ShaderResourceViewDesc& desc)
    {
        auto shaderResourceView = std::make_shared<ShaderResourceView>(this, desc);
        m_deviceObjects.push_back(shaderResourceView);
        return shaderResourceView;
    }

    std::shared_ptr<ShaderRWResourceView> Device::CreateShaderRWResourceView(const ShaderRWResourceViewDesc& desc)
    {
        auto shaderRWResourceView = std::make_shared<ShaderRWResourceView>(this, desc);
        m_deviceObjects.push_back(shaderRWResourceView);
        return shaderRWResourceView;
    }

    std::shared_ptr<RenderTargetView> Device::CreateRenderTargetView(const RenderTargetViewDesc& desc)
    {
        auto renderTargetView = std::make_shared<RenderTargetView>(this, desc);
        m_deviceObjects.push_back(renderTargetView);
        return renderTargetView;
    }

    std::shared_ptr<DepthStencilView> Device::CreateDepthStencilView(const DepthStencilViewDesc& desc)
    {
        auto depthStencilView = std::make_shared<DepthStencilView>(this, desc);
        m_deviceObjects.push_back(depthStencilView);
        return depthStencilView;
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
