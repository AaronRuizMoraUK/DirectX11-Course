#include <Renderer/Renderer.h>

#include <RHI/Device/Device.h>
#include <RHI/Device/DeviceContext.h>
#include <RHI/SwapChain/SwapChain.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Pipeline/Pipeline.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Shader/Shader.h>
#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>
#include <RHI/Pipeline/PipelineResourceBindings.h>

#include <Window/Window.h>
#include <File/FileUtils.h>
#include <Log/Log.h>

namespace DX
{
    Renderer::Renderer(RendererId rendererId, Window* window)
        : m_rendererId(rendererId)
        , m_window(window)
    {
    }

    Renderer::~Renderer()
    {
        Terminate();
    }

    bool Renderer::Initialize()
    {
        if (m_device)
        {
            return true; // Already initialized
        }

        DX_LOG(Info, "Renderer", "Initializing Renderer...");

        if (!CreateDevice())
        {
            Terminate();
            return false;
        }

        if (!CreateSwapChain())
        {
            Terminate();
            return false;
        }

        if (!CreateFrameBuffer())
        {
            Terminate();
            return false;
        }

        if (!CreatePipeline())
        {
            Terminate();
            return false;
        }

        return true;
    }

    void Renderer::Terminate()
    {
        DX_LOG(Info, "Renderer", "Terminating Renderer...");

        DestroyPipeline();

        m_frameBuffer.reset();
        m_swapChain.reset();
        m_device.reset();
    }

    Window* Renderer::GetWindow()
    {
        return m_window;
    }

    Device* Renderer::GetDevice()
    {
        return m_device.get();
    }

    bool Renderer::CreateDevice()
    {
        m_device = std::make_unique<Device>();

        if (!m_device)
        {
            DX_LOG(Error, "Renderer", "Failed to create device.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Device created.");
        return true;
    }

    bool Renderer::CreateSwapChain()
    {
        SwapChainDesc swapChainDesc = {};
        swapChainDesc.m_size = m_window->GetSize();
        swapChainDesc.m_refreshRate = m_window->GetRefreshRate();
        swapChainDesc.m_fullScreen = m_window->IsFullScreen();
        swapChainDesc.m_vSyncEnabled = m_window->IsVSyncEnabled();
        swapChainDesc.m_bufferCount = 1;
        swapChainDesc.m_bufferFormat = ResourceFormat::R8G8B8A8_UNORM;
        swapChainDesc.m_nativeWindowHandler = m_window->GetWindowNativeHandler();

        m_swapChain = m_device->CreateSwapChain(swapChainDesc);

        if (!m_swapChain)
        {
            DX_LOG(Error, "Renderer", "Failed to create swap chain.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Swap chain created.");
        return true;
    }

    bool Renderer::CreateFrameBuffer()
    {
        FrameBufferDesc frameBufferDesc = {};
        frameBufferDesc.m_renderTargetAttachments = FrameBufferDesc::TextureAttachments{ 
            {m_swapChain->GetBackBufferTexture(), m_swapChain->GetBackBufferTexture()->GetTextureDesc().m_format}
        };
        frameBufferDesc.m_createDepthStencilAttachment = true;

        m_frameBuffer = m_device->CreateFrameBuffer(frameBufferDesc);

        if (!m_swapChain)
        {
            DX_LOG(Error, "Renderer", "Failed to create frame buffer.");
            return false;
        }

        DX_LOG(Info, "Renderer", "Swap chain created.");
        return true;
    }

    void Renderer::Clear(const Math::Color& color, float depth, uint8_t stencil)
    {
        m_device->GetImmediateContext().ClearFrameBuffer(*m_frameBuffer, color, depth, stencil);
    }

    void Renderer::Present()
    {
        m_swapChain->Present();
    }

    void Renderer::BindPipeline()
    {
        m_device->GetImmediateContext().BindFrameBuffer(*m_frameBuffer);
        m_device->GetImmediateContext().BindViewports({ Math::Rectangle{{0.0f, 0.0f}, Math::Vector2{m_window->GetSize()}} });

        m_device->GetImmediateContext().BindPipeline(*m_pipeline);
    }

    void Renderer::BindPipelineResources()
    {
        m_device->GetImmediateContext().BindResources(*m_pipelineResources);
    }

    PipelineResourceBindings& Renderer::GetPipelineResources()
    {
        return *m_pipelineResources;
    }

    void Renderer::Draw(int indexCount)
    {
        m_device->GetImmediateContext().DrawIndexed(indexCount, 0, 0);
    }

    bool Renderer::CreatePipeline()
    {
        const ShaderInfo vertexShaderInfo{ ShaderType_Vertex, "Shaders/VertexShader.hlsl", "main" };
        const ShaderInfo pixelShaderInfo{ ShaderType_Pixel, "Shaders/PixelShader.hlsl", "main" };
        auto vertexShaderByteCode = ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = ShaderCompiler::Compile(pixelShaderInfo);
        auto vertexShader = m_device->CreateShader({ vertexShaderInfo, vertexShaderByteCode });
        auto pixelShader = m_device->CreateShader({ pixelShaderInfo, pixelShaderByteCode });

        PipelineDesc pipelineDesc = {};
        pipelineDesc.m_shaders[DX::ShaderType_Vertex] = vertexShader;
        pipelineDesc.m_shaders[DX::ShaderType_Pixel] = pixelShader;
        pipelineDesc.m_inputLayout.m_inputElements =
        {
            InputElement{ DX::InputSemantic::Position, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 0 },
            //InputElement{ DX::InputSemantic::Color, 0, DX::ResourceFormat::R32G32B32A32_FLOAT, 0, 12 },
            InputElement{ DX::InputSemantic::TexCoord, 0, DX::ResourceFormat::R32G32_FLOAT, 0, 12 },
        };
        pipelineDesc.m_inputLayout.m_primitiveTopology = DX::PrimitiveTopology::TriangleList;
        pipelineDesc.m_rasterizerState = {
            .m_faceFrontOrder = DX::FaceFrontOrder::Clockwise,
            .m_faceCullMode = DX::FaceCullMode::BackFace,
            .m_faceFillMode = DX::FaceFillMode::Solid,
        };
        pipelineDesc.m_blendState.renderTargetBlends[0] = {
            .m_blendEnabled = false,
            .m_colorWriteMask = DX::ColorWrite_All
        };
        pipelineDesc.m_depthStencilState = {
            .m_depthEnabled = true,
            .m_depthTestFunc = DX::ComparisonFunction::Less,
            .m_depthWriteEnabled = true,
            .m_stencilEnabled = false
        };

        m_pipeline = m_device->CreatePipeline(pipelineDesc);

        if (!m_pipeline)
        {
            DX_LOG(Error, "Renderer", "Failed to create pipeline.");
            return false;
        }

        m_pipelineResources = std::make_unique<PipelineResourceBindings>(m_pipeline->GetResourceBindings());

        DX_LOG(Info, "Renderer", "Pipeline created.");
        return true;
    }

    void Renderer::DestroyPipeline()
    {
        m_pipelineResources.reset();
        m_pipeline.reset();
    }
} // namespace DX
