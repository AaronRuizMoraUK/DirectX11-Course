#include <RHI/Device/Device.h>

#include <RHI/Device/DeviceContext.h>
#include <RHI/SwapChain/SwapChain.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Sampler/Sampler.h>
#include <RHI/Shader/Shader.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Resource/Views/ShaderRWResourceView.h>
#include <RHI/Resource/Views/RenderTargetView.h>
#include <RHI/Resource/Views/DepthStencilView.h>
#include <RHI/Pipeline/Pipeline.h>
#include <RHI/ResourceLayout/ResourceLayout.h>
#include <RHI/CommandList/CommandList.h>

#include <Log/Log.h>

#include <array>
#include <algorithm>

#include <d3d11.h>

namespace DX
{
    Device::Device()
    {
        const std::array<D3D_FEATURE_LEVEL, 1> featureLevels = { D3D_FEATURE_LEVEL_11_1 };

        uint32_t flags = 0;
#ifdef _DEBUG
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        ComPtr<ID3D11DeviceContext> dx11DeviceContext;

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
            dx11DeviceContext.GetAddressOf()
        );

        if (FAILED(result))
        {
            DX_LOG(Fatal, "Device", "Failed to create D3D11 device.");
            return;
        }

        // Create the immediate context.
        m_immediateContext = std::make_unique<DeviceContext>(this, DeviceContextType::Immediate, dx11DeviceContext.Get());
        if (!m_immediateContext)
        {
            DX_LOG(Fatal, "Device", "Failed to create immediate device context.");
            return;
        }

        DX_LOG(Info, "Device", "Graphics device created.");
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
                DX_LOG(Warning, "Device", "There are %d device objects still referenced at the time of destroying device.", m_deviceObjects.size());
            }
        }
#endif

        m_deviceObjects.clear();

        m_immediateContext.reset();

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

    std::shared_ptr<CommandList> Device::CreateCommandList()
    {
        auto commandList = std::make_shared<CommandList>(this);
        m_deviceObjects.push_back(commandList);
        return commandList;
    }

    void Device::ExecuteCommandLists(std::vector<CommandList*> commandLists)
    {
        const bool restoreContextState = false;
        for (auto* commandList : commandLists)
        {
            // Immediate context state is cleared before and after a command list is executed.
            // A command list has no concept of inheritance.
            m_immediateContext->GetDX11DeviceContext()->ExecuteCommandList(
                commandList->GetDX11CommandList().Get(), restoreContextState);

            commandList->ClearCommandList();
        }
    }

    DeviceContext& Device::GetImmediateContext()
    {
        return *m_immediateContext;
    }

    ComPtr<ID3D11Device> Device::GetDX11Device()
    {
        return m_dx11Device;
    }
} // namespace DX
