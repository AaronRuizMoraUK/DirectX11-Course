#define SIMPLE_RENDERER 0

#if SIMPLE_RENDERER
#include <Renderer/RendererManager.h>
#include <Renderer/Object.h>
#include <Renderer/Camera.h>
#else
#include <Graphics/Device/DeviceManager.h>
#include <Graphics/SwapChain/SwapChain.h>
#include <Graphics/FrameBuffer/FrameBuffer.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Shader/ShaderCompiler.h>
#include <Graphics/Sampler/Sampler.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#include <Graphics/Resource/Views/RenderTargetView.h>
#include <Graphics/Resource/Views/ShaderResourceView.h>
#include <Graphics/Resource/Views/ShaderRWResourceView.h>
#endif

#include <Window/WindowManager.h>
#include <Log/Log.h>
#include <Math/Color.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <File/FileUtils.h>

#include <array>
#include <memory>
#include <numeric>

int main()
{
    //const mathfu::Vector2Int windowSize{ 3440, 1440 };
    //const int refreshRate = 144;
    //const bool fullScreen = true;
    //const bool vSync = true;

    const mathfu::Vector2Int windowSize{ 1280, 720 };
    const int refreshRate = 144;
    const bool fullScreen = false;
    const bool vSync = true;

    // Window initialization
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle("DirectX11 Course", windowSize, refreshRate, fullScreen, vSync);
    if (!window)
    {
        return -1;
    }

#if SIMPLE_RENDERER

    // Renderer initialization
    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.CreateRenderer(window);
    if (!renderer)
    {
        return -1;
    }

    // Camera
    auto camera = std::make_unique<DX::Camera>(mathfu::Vector3(2.0f, 1.0f, -2.0f), mathfu::Vector3(0.0f));

    // Rendering objects initialization
    std::vector<std::unique_ptr<DX::Object>> objects;
    objects.push_back(std::make_unique<DX::Triangle>());
    objects.push_back(std::make_unique<DX::Cube>(mathfu::Vector3(1.0f)));
    objects[0]->SetTransform(mathfu::Vector3(0.0f, 1.0f, 0.0f));

    while (window->IsOpen())
    {
        windowManager.PollEvents();

        // ------
        // Update
        // ------
        constexpr float deltaTime = 1.0f / static_cast<float>(refreshRate); // TODO: Calculate delta time
        camera->Update(deltaTime);
        //for (auto& object : objects)
        //{
        //    mathfu::Transform& transform = object->GetTransform();
        //    transform.m_rotation = transform.m_rotation * mathfu::Quat::FromEulerAngles(mathfu::Vector3(0.1f * deltaTime));
        //}

        // ------
        // Render
        // ------
        const Math::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
        renderer->ClearColor(clearColor);

        renderer->SetPipeline();

        camera->SetBuffers();

        for (auto& object : objects)
        {
            object->SetBuffers();

            renderer->Draw(object->GetIndexCount());
        }

        renderer->Present();
    }

    objects.clear();
    camera.reset();
    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();

#else

    // Graphics device initialization
    DX::DeviceManager& deviceManager = DX::DeviceManager::Get();
    DX::Device* device = deviceManager.CreateDevice({});
    if (!device)
    {
        return -1;
    }

    // Tests creating all device objects
    {
        auto swapChain = device->CreateSwapChain({ window, 1, DX::ResourceFormat::R8G8B8A8_UNORM });
        auto frameBuffer = device->CreateFrameBuffer({ swapChain->GetBackBufferTexture(), nullptr, true /*Create Depth Stencil*/});

        const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType::Vertex, "Shaders/VertexShader.hlsl", "main" };
        const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType::Pixel, "Shaders/PixelShader.hlsl", "main" };

        auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);

        auto vertexShader = device->CreateShader({ vertexShaderInfo, vertexShaderByteCode.get() });
        auto pixelShader = device->CreateShader({ pixelShaderInfo, pixelShaderByteCode.get() });

        auto sampler = device->CreateSampler({});

        mathfu::Vector2Int textureSize;
        auto* textureData = DX::LoadTexture("Textures/Wall_Stone_Albedo.png", textureSize);

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0.0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData;

        auto texture = device->CreateTexture(textureDesc);

        auto textureSRV = device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        //auto textureSRWRV = device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        //auto textureRTV = device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });

        std::vector<float> bufferData(10);
        std::iota(bufferData.begin(), bufferData.end(), 1.0f);

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Typed;
        bufferDesc.m_sizeInBytes = bufferData.size() * sizeof(float);
        bufferDesc.m_usage = DX::ResourceUsage::Immutable;
        bufferDesc.m_bindFlags = DX::BufferBind_ShaderResource;
        bufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        bufferDesc.m_initialData = bufferData.data();

        auto buffer = device->CreateBuffer(bufferDesc);

        DX::ShaderResourceViewDesc bufferSRVDesc = {};
        bufferSRVDesc.m_resource = buffer;
        bufferSRVDesc.m_viewFormat = DX::ResourceFormat::R32_FLOAT;
        bufferSRVDesc.m_firstElement = 0;
        bufferSRVDesc.m_elementCount = bufferData.size();

        DX::ShaderRWResourceViewDesc bufferSRWRVDesc = {};
        bufferSRWRVDesc.m_resource = buffer;
        bufferSRWRVDesc.m_viewFormat = DX::ResourceFormat::R32_FLOAT;
        bufferSRWRVDesc.m_firstElement = 0;
        bufferSRWRVDesc.m_elementCount = bufferData.size();

        DX::RenderTargetViewDesc bufferRTVDesc = {};
        bufferRTVDesc.m_resource = buffer;
        bufferRTVDesc.m_viewFormat = DX::ResourceFormat::R32_FLOAT;
        bufferRTVDesc.m_firstElement = 0;
        bufferRTVDesc.m_elementCount = bufferData.size();

        auto bufferRSV = device->CreateShaderResourceView(bufferSRVDesc);
        //auto bufferSRWRSV = device->CreateShaderRWResourceView(bufferSRWRVDesc);
        //auto bufferRTV = device->CreateRenderTargetView(bufferRTVDesc);

        while (window->IsOpen())
        {
            windowManager.PollEvents();

            // ------
            // Update
            // ------

            // ------
            // Render
            // ------
            const mathfu::Color clearColor(0.2f, 0.0f, 0.3f, 1.0f);
            frameBuffer->Clear(clearColor, 0.0f, 0);

            swapChain->Present();
        }
    }

    DX::DeviceManager::Destroy();
    DX::WindowManager::Destroy();

#endif // SIMPLE_RENDERER

    DX_LOG(Info, "Main", "Done!");
    return 0;
}
