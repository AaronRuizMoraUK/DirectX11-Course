
#include <Graphics/Device/DeviceManager.h>
#include <Graphics/Shader/Shader.h>
#include <Graphics/Shader/ShaderCompiler.h>
#include <Graphics/Sampler/Sampler.h>
#include <Graphics/Resource/Texture/Texture.h>
#include <Graphics/Resource/Buffer/Buffer.h>
#include <Graphics/Resource/Views/RenderTargetView.h>
#include <Graphics/Resource/Views/ShaderResourceView.h>
#include <Graphics/Resource/Views/ShaderRWResourceView.h>

#include <Log/Log.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <File/FileUtils.h>

#include <numeric>

namespace UnitTest
{
    class DeviceObjectTests
    {
    public:
        DeviceObjectTests(DX::Device* device)
            : m_device(device)
        {
        }

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

    private:
        DX::Device* m_device = nullptr;
    };

    void TestsDeviceObjects()
    {
        DeviceObjectTests tests(DX::DeviceManager::Get().GetDevice());

        tests.TestShader();
        tests.TestSampler();
        tests.TestTexure1D();
        tests.TestTexure1DArray();
        tests.TestTexure2D();
        tests.TestTexure2DArray();
        tests.TestTexureCube();
        tests.TestTexureCubeArray();
        tests.TestTexure3D();
        tests.TestTypedBuffer();
        tests.TestStructuredBuffer();
        tests.TestRawBuffer();
    }

    void DeviceObjectTests::TestShader()
    {
        DX_LOG(Info, "Test", " ----- Testing Shader -----");

        const DX::ShaderInfo vertexShaderInfo{ DX::ShaderType::Vertex, "Shaders/VertexShader.hlsl", "main" };
        const DX::ShaderInfo pixelShaderInfo{ DX::ShaderType::Pixel, "Shaders/PixelShader.hlsl", "main" };

        auto vertexShaderByteCode = DX::ShaderCompiler::Compile(vertexShaderInfo);
        auto pixelShaderByteCode = DX::ShaderCompiler::Compile(pixelShaderInfo);

        auto vertexShader = m_device->CreateShader({ vertexShaderInfo, vertexShaderByteCode.get() });
        auto pixelShader = m_device->CreateShader({ pixelShaderInfo, pixelShaderByteCode.get() });
    }

    void DeviceObjectTests::TestSampler()
    {
        DX_LOG(Info, "Test", " ----- Testing Sampler -----");

        auto sampler = m_device->CreateSampler({});
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
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0, 0);
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
                const int index = (row + arrayIndex * textureSize) * components;
                textureData[index + 0] = value;
                textureData[index + 1] = value;
                textureData[index + 2] = value;
                textureData[index + 3] = value;
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture1D;
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0, 0);
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

        mathfu::Vector2Int textureSize;
        auto* textureData = DX::LoadTexture("Textures/Wall_Stone_Albedo.png", textureSize);

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0);
        textureDesc.m_mipCount = 1;
        textureDesc.m_format = DX::ResourceFormat::R8G8B8A8_UNORM;
        textureDesc.m_usage = DX::ResourceUsage::Default; // DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource | DX::TextureBind_ShaderRWResource | DX::TextureBind_RenderTarget;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData;

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTexure2DArray()
    {
        DX_LOG(Info, "Test", " ----- Testing Texure2D Array -----");

        const int components = 4;
        mathfu::Vector2Int textureSize(256, 256);
        const int arrayCount = 3;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * arrayCount);
        for (int arrayIndex = 0; arrayIndex < arrayCount; ++arrayIndex)
        {
            for (int col = 0; col < textureSize.y; ++col)
            {
                for (int row = 0; row < textureSize.x; ++row)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index = (row 
                        + col * (textureSize.x) 
                        + arrayIndex * (textureSize.x * textureSize.y)) * components;
                    textureData[index + 0] = value;
                    textureData[index + 1] = value;
                    textureData[index + 2] = value;
                    textureData[index + 3] = value;
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::Texture2D;
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0);
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
        mathfu::Vector2Int textureSize(256, 256);
        const int faceCount = 6;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * faceCount);
        for (int faceIndex = 0; faceIndex < faceCount; ++faceIndex)
        {
            for (int col = 0; col < textureSize.y; ++col)
            {
                for (int row = 0; row < textureSize.x; ++row)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index = (row 
                        + col * (textureSize.x) 
                        + faceIndex * (textureSize.x * textureSize.y)) * components;
                    textureData[index + 0] = value;
                    textureData[index + 1] = value;
                    textureData[index + 2] = value;
                    textureData[index + 3] = value;
                }
            }
        }

        DX::TextureDesc textureDesc = {};
        textureDesc.m_textureType = DX::TextureType::TextureCube;
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0);
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
        mathfu::Vector2Int textureSize(256, 256);
        const int faceCount = 6;
        const int arrayCount = 3;
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * faceCount * arrayCount);
        for (int arrayIndex = 0; arrayIndex < arrayCount; ++arrayIndex)
        {
            for (int faceIndex = 0; faceIndex < faceCount; ++faceIndex)
            {
                for (int col = 0; col < textureSize.y; ++col)
                {
                    for (int row = 0; row < textureSize.x; ++row)
                    {
                        const std::byte value = static_cast<std::byte>(row % 255);
                        const int index = (row
                            + col * (textureSize.x) 
                            + faceIndex * (textureSize.x * textureSize.y)
                            + arrayIndex * (textureSize.x * textureSize.y * faceCount)) * components;
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
        textureDesc.m_dimensions = mathfu::Vector3Int(textureSize, 0);
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
        mathfu::Vector3Int textureSize(256, 256, 256);
        std::vector<std::byte> textureData(components * textureSize.x * textureSize.y * textureSize.z);
        for (int depth = 0; depth < textureSize.z; ++depth)
        {
            for (int col = 0; col < textureSize.y; ++col)
            {
                for (int row = 0; row < textureSize.x; ++row)
                {
                    const std::byte value = static_cast<std::byte>(row % 255);
                    const int index = (row 
                        + col * (textureSize.x) 
                        + depth * (textureSize.x * textureSize.y)) * components;
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
        bufferDesc.m_sizeInBytes = bufferData.size() * sizeof(float);
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
        bufferDesc.m_sizeInBytes = bufferData.size() * sizeof(MyBuffer);
        bufferDesc.m_structSizeInBytes = sizeof(MyBuffer);
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

        const int components = 4;
        const int bufferSize = 256;
        std::vector<std::byte> bufferData(components * bufferSize);
        for (int i = 0; i < bufferSize; ++i)
        {
            const std::byte value = static_cast<std::byte>(i % 255);
            const int index = i * components;
            bufferData[index + 0] = value;
            bufferData[index + 1] = value;
            bufferData[index + 2] = value;
            bufferData[index + 3] = value;
        }

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Raw;
        bufferDesc.m_sizeInBytes = bufferData.size() * sizeof(std::byte);
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
}
