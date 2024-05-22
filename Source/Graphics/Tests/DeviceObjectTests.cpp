
#include <RHI/Device/Device.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Shader/Shader.h>
#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>
#include <RHI/Sampler/Sampler.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Views/RenderTargetView.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Resource/Views/ShaderRWResourceView.h>
#include <RHI/Pipeline/Pipeline.h>

#include <Log/Log.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>

#include <numeric>

namespace UnitTest
{
    class DeviceObjectTests
    {
    public:
        DeviceObjectTests(DX::Device* device)
            : m_device(device)
        {
            TestFrameBuffer();
            TestShader();
            TestSampler();
            TestTexure1D();
            TestTexure1DArray();
            TestTexure2D();
            TestTexure2DArray();
            TestTexureCube();
            TestTexureCubeArray();
            TestTexure3D();
            TestTypedBuffer();
            TestStructuredBuffer();
            TestRawBuffer();
            TestPipeline();
        }

    private:
        void TestFrameBuffer();
        void TestShader();
        void TestSampler();
        void TestTexure1D();
        void TestTexure1DArray();
        void TestTexure2D();
        void TestTexure2DArray();
        void TestTexureCube();
        void TestTexureCubeArray();
        void TestTexure3D();
        void TestTypedBuffer();
        void TestStructuredBuffer();
        void TestRawBuffer();
        void TestPipeline();

        DX::Device* m_device = nullptr;
    };

    void TestsDeviceObjects()
    {
        // Graphics device
        std::unique_ptr<DX::Device> device = std::make_unique<DX::Device>();
        if (!device)
        {
            return;
        }

        DeviceObjectTests tests(device.get());

        DX_LOG(Info, "Test", " --------------------------");
    }

    void DeviceObjectTests::TestFrameBuffer()
    {
        DX_LOG(Info, "Test", " ----- Testing FrameBuffer -----");

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = Math::Vector3Int(1280, 720, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default;
        textureDesc.m_bindFlags = DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = nullptr;

        auto texture = m_device->CreateTexture(textureDesc);

        DX::FrameBufferDesc frameBufferDesc = {};
        frameBufferDesc.m_colorAttachment = texture;
        frameBufferDesc.m_createDepthStencilAttachment = true;

        auto frameBuffer = m_device->CreateFrameBuffer(frameBufferDesc);
    }

    void DeviceObjectTests::TestShader()
    {
        DX_LOG(Info, "Test", " ----- Testing Shader -----");

        const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType_Vertex, "Shaders/Tests/VertexShaderTest.hlsl", "main" };
        const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType_Pixel, "Shaders/Tests/PixelShaderTest.hlsl", "main" };

        auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);

        auto vertexShader = m_device->CreateShader({ vertexShaderInfo, vertexShaderByteCode });
        auto pixelShader = m_device->CreateShader({ pixelShaderInfo, pixelShaderByteCode });
    }

    void DeviceObjectTests::TestSampler()
    {
        DX_LOG(Info, "Test", " ----- Testing Sampler -----");

        DX::SamplerDesc samplerDesc;
        samplerDesc.m_minFilter = DX::FilterSampling::Linear;
        samplerDesc.m_magFilter = DX::FilterSampling::Linear;
        samplerDesc.m_mipFilter = DX::FilterSampling::Linear;
        samplerDesc.m_filterMode = DX::FilterMode::Normal;
        samplerDesc.m_addressU = DX::AddressMode::Wrap;
        samplerDesc.m_addressV = DX::AddressMode::Wrap;
        samplerDesc.m_addressW = DX::AddressMode::Wrap;
        samplerDesc.m_mipBias = 0.0f;
        samplerDesc.m_mipClamp = DX::NoMipClamping;
        samplerDesc.m_maxAnisotropy = 1;
        samplerDesc.m_borderColor = Math::Color(0.0f);
        samplerDesc.m_comparisonFunction = DX::ComparisonFunction::Always;

        auto sampler = m_device->CreateSampler(samplerDesc);
    }

    void DeviceObjectTests::TestTexure1D()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure1D -----");

        const int components = 4;
        const int textureSize = 256;
        std::vector<std::byte> textureData(components * textureSize);
        for (int row = 0; row < textureSize; ++row)
        {
            const std::byte value = static_cast<std::byte>(row % 255);
            const int index = row * components;
            textureData[index + 0] = value;
            textureData[index + 1] = value;
            textureData[index + 2] = value;
            textureData[index + 3] = value;
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture1D;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTexure1DArray()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure1D Array -----");

        const int components = 4;
        const int textureSize = 256;
        const int arrayCount = 3;
        std::vector<std::byte> textureData(components * textureSize * arrayCount);
        for (int arrayIndex = 0; arrayIndex < arrayCount; ++arrayIndex)
        {
            for (int row = 0; row < textureSize; ++row)
            {
                const std::byte value = static_cast<std::byte>(row % 255);
                const int index = (arrayIndex * textureSize + row) * components;
                textureData[index + 0] = value;
                textureData[index + 1] = value;
                textureData[index + 2] = value;
                textureData[index + 3] = value;
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture1D;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = arrayCount;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1, 0, arrayCount });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0, 0, arrayCount });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0, 0, arrayCount });
    }

    void DeviceObjectTests::TestTexure2D()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure2D -----");

        const int components = 4;
        Math::Vector2Int textureSize(256, 256);
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y);
        for (int row = 0; row < textureSize.x; ++row)
        {
            for (int col = 0; col < textureSize.y; ++col)
            {
                const std::byte value = static_cast<std::byte>(row % 255);
                const int index = (row * textureSize.y + col) * components;
                textureData[index + 0] = value;
                textureData[index + 1] = value;
                textureData[index + 2] = value;
                textureData[index + 3] = value;
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTexure2DArray()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure2D Array -----");

        const int components = 4;
        Math::Vector2Int textureSize(256, 256);
        const int arrayCount = 3;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * arrayCount);
        for (int arrayIndex = 0; arrayIndex < arrayCount; ++arrayIndex)
        {
            for (int row = 0; row < textureSize.x; ++row)
            {
                for (int col = 0; col < textureSize.y; ++col)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index = (arrayIndex * textureSize.x * textureSize.y
                        + row * textureSize.y + col) * components;
                    textureData[index + 0] = value;
                    textureData[index + 1] = value;
                    textureData[index + 2] = value;
                    textureData[index + 3] = value;
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = arrayCount;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1, 0, arrayCount });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0, 0, arrayCount });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0, 0, arrayCount });
    }

    void DeviceObjectTests::TestTexureCube()
    {
        DX_LOG(Info, "Test", " ----- Testing TexureCube -----");

        const int components = 4;
        Math::Vector2Int textureSize(256, 256);
        const int faceCount = 6;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * faceCount);
        for (int faceIndex = 0; faceIndex < faceCount; ++faceIndex)
        {
            for (int row = 0; row < textureSize.x; ++row)
            {
                for (int col = 0; col < textureSize.y; ++col)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index =(faceIndex * textureSize.x * textureSize.y
                        + row * textureSize.y + col) * components;
                    textureData[index + 0] = value;
                    textureData[index + 1] = value;
                    textureData[index + 2] = value;
                    textureData[index + 3] = value;
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::TextureCube;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = faceCount;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        // Technically Shader RW Resource View and Render Target View do not support texture Cube,
        // but we can still create the view to read it as a Texture 2D Array.
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0, 0, faceCount });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0, 0, faceCount });
    }

    void DeviceObjectTests::TestTexureCubeArray()
    {
        DX_LOG(Info, "Test", " ----- Testing TexureCube Array -----");

        const int components = 4;
        Math::Vector2Int textureSize(256, 256);
        const int faceCount = 6;
        const int arrayCount = 3;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * faceCount * arrayCount);
        for (int arrayIndex = 0; arrayIndex < arrayCount; ++arrayIndex)
        {
            for (int faceIndex = 0; faceIndex < faceCount; ++faceIndex)
            {
                for (int row = 0; row < textureSize.x; ++row)
                {
                    for (int col = 0; col < textureSize.y; ++col)
                    {
                        const std::byte value = static_cast<std::byte>(row % 255);
                        const int index = (arrayIndex * faceCount * textureSize.x * textureSize.y
                            + faceIndex * textureSize.x * textureSize.y
                            + row * textureSize.y + col) * components;
                        textureData[index + 0] = value;
                        textureData[index + 1] = value;
                        textureData[index + 2] = value;
                        textureData[index + 3] = value;
                    }
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::TextureCube;
        textureDesc.m_dimensions = Math::Vector3Int(textureSize, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = faceCount * arrayCount;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1, 0, arrayCount });
        // Technically Shader RW Resource View and Render Target View do not support texture Cube Array,
        // but we can still create the view to read it as a Texture 2D Array.
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0, 0, faceCount * arrayCount });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0, 0, faceCount * arrayCount });
    }

    void DeviceObjectTests::TestTexure3D()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure3D -----");

        const int components = 4;
        Math::Vector3Int textureSize(256, 256, 256);
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * textureSize.z);
        for (int depth = 0; depth < textureSize.z; ++depth)
        {
            for (int row = 0; row < textureSize.x; ++row)
            {
                for (int col = 0; col < textureSize.y; ++col)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index = (depth * textureSize.x * textureSize.y
                        + row * textureSize.y + col) * components;
                    textureData[index + 0] = value;
                    textureData[index + 1] = value;
                    textureData[index + 2] = value;
                    textureData[index + 3] = value;
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture3D;
        textureDesc.m_dimensions = textureSize;
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0, 0, 0, 0, static_cast<uint32_t>(textureSize.z) });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0, 0, 0, 0, static_cast<uint32_t>(textureSize.z) });
    }

    void DeviceObjectTests::TestTypedBuffer()
    {
        DX_LOG(Info, "Test", " ----- Testing TypedBuffer -----");

        std::vector<float> bufferData(256);
        std::iota(bufferData.begin(), bufferData.end(), 1.0f);

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Typed;
        bufferDesc.m_elementSizeInBytes = sizeof(float);
        bufferDesc.m_elementCount = bufferData.size();
        bufferDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        bufferDesc.m_bindFlags = DX::BufferBind_ShaderResource | DX::BufferBind_ShaderRWResource | DX::BufferBind_RenderTarget;
        bufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        bufferDesc.m_initialData = bufferData.data();

        auto buffer = m_device->CreateBuffer(bufferDesc);

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

        auto bufferRSV = m_device->CreateShaderResourceView(bufferSRVDesc);
        auto bufferSRWRSV = m_device->CreateShaderRWResourceView(bufferSRWRVDesc);
        auto bufferRTV = m_device->CreateRenderTargetView(bufferRTVDesc);
    }

    void DeviceObjectTests::TestStructuredBuffer()
    {
        DX_LOG(Info, "Test", " ----- Testing StructuredBuffer -----");

        struct MyBuffer
        {
            int a, b, c;
            float d, e, f;
        };

        std::vector<MyBuffer> bufferData(256);
        for (int i = 0; i < bufferData.size(); ++i)
        {
            const float fpi = static_cast<float>(i);
            bufferData[i] = MyBuffer{ i, i, i, fpi, fpi, fpi };
        }

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Structured;
        bufferDesc.m_elementSizeInBytes = sizeof(MyBuffer);
        bufferDesc.m_elementCount = bufferData.size();
        bufferDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        bufferDesc.m_bindFlags = DX::BufferBind_ShaderResource | DX::BufferBind_ShaderRWResource;
        bufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        bufferDesc.m_initialData = bufferData.data();

        auto buffer = m_device->CreateBuffer(bufferDesc);

        DX::ShaderResourceViewDesc bufferSRVDesc = {};
        bufferSRVDesc.m_resource = buffer;
        bufferSRVDesc.m_viewFormat = DX::ResourceFormat::Unknown;
        bufferSRVDesc.m_firstElement = 0;
        bufferSRVDesc.m_elementCount = bufferData.size();

        DX::ShaderRWResourceViewDesc bufferSRWRVDesc = {};
        bufferSRWRVDesc.m_resource = buffer;
        bufferSRWRVDesc.m_viewFormat = DX::ResourceFormat::Unknown;
        bufferSRWRVDesc.m_firstElement = 0;
        bufferSRWRVDesc.m_elementCount = bufferData.size();

        DX::RenderTargetViewDesc bufferRTVDesc = {};
        bufferRTVDesc.m_resource = buffer;
        bufferRTVDesc.m_viewFormat = DX::ResourceFormat::Unknown;
        bufferRTVDesc.m_firstElement = 0;
        bufferRTVDesc.m_elementCount = bufferData.size();

        auto bufferRSV = m_device->CreateShaderResourceView(bufferSRVDesc);
        auto bufferSRWRSV = m_device->CreateShaderRWResourceView(bufferSRWRVDesc);
        // RenderTargetView is not supported for structured buffers
    }

    void DeviceObjectTests::TestRawBuffer()
    {
        DX_LOG(Info, "Test", " ----- Testing RawBuffer -----");

        const int bufferSize = 256;
        std::vector<uint32_t> bufferData(bufferSize);
        for (int i = 0; i < bufferSize; ++i)
        {
            bufferData[i] = i;
        }

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Raw;
        bufferDesc.m_elementSizeInBytes = sizeof(uint32_t);
        bufferDesc.m_elementCount = bufferData.size();
        bufferDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        bufferDesc.m_bindFlags = DX::BufferBind_ShaderResource | DX::BufferBind_ShaderRWResource;
        bufferDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        bufferDesc.m_initialData = bufferData.data();

        auto buffer = m_device->CreateBuffer(bufferDesc);

        DX::ShaderResourceViewDesc bufferSRVDesc = {};
        bufferSRVDesc.m_resource = buffer;
        bufferSRVDesc.m_viewFormat = DX::ResourceFormat::R32_TYPELESS;
        bufferSRVDesc.m_firstElement = 0;
        bufferSRVDesc.m_elementCount = bufferSize;

        DX::ShaderRWResourceViewDesc bufferSRWRVDesc = {};
        bufferSRWRVDesc.m_resource = buffer;
        bufferSRWRVDesc.m_viewFormat = DX::ResourceFormat::R32_TYPELESS;
        bufferSRWRVDesc.m_firstElement = 0;
        bufferSRWRVDesc.m_elementCount = bufferSize;

        DX::RenderTargetViewDesc bufferRTVDesc = {};
        bufferRTVDesc.m_resource = buffer;
        bufferRTVDesc.m_viewFormat = DX::ResourceFormat::R32_TYPELESS;
        bufferRTVDesc.m_firstElement = 0;
        bufferRTVDesc.m_elementCount = bufferSize;

        auto bufferRSV = m_device->CreateShaderResourceView(bufferSRVDesc);
        auto bufferSRWRSV = m_device->CreateShaderRWResourceView(bufferSRWRVDesc);
        // RenderTargetView is not supported for raw buffers
    }

    void DeviceObjectTests::TestPipeline()
    {
        DX_LOG(Info, "Test", " ----- Testing Pipeline -----");

        const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType_Vertex, "Shaders/VertexShader.hlsl", "mainColor" };
        const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType_Pixel, "Shaders/PixelShader.hlsl", "mainColor" };
        auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);
        auto vertexShader = m_device->CreateShader({ vertexShaderInfo, vertexShaderByteCode });
        auto pixelShader = m_device->CreateShader({ pixelShaderInfo, pixelShaderByteCode });

        DX::PipelineDesc pipelineDesc = {};
        pipelineDesc.m_shaders[DX::ShaderType_Vertex] = vertexShader;
        pipelineDesc.m_shaders[DX::ShaderType_Pixel] = pixelShader;
        pipelineDesc.m_inputLayout.m_inputElements =
        {
            DX::InputElement{ DX::InputSemantic::Position, 0, DX::ResourceFormat::R32G32B32_FLOAT, 0, 0 },
            DX::InputElement{ DX::InputSemantic::Color, 0, DX::ResourceFormat::R32G32B32A32_FLOAT, 0, 12 },
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

        auto pipeline = m_device->CreatePipeline(pipelineDesc);
    }
}
