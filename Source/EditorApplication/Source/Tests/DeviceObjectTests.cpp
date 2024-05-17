
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
        void TestTexure3D();
        void TestTypedBuffer();
        void TestStructuredBuffer();
        void TestRawBuffer();

    private:
        DX::Device* m_device = nullptr;
    };

    void TestsDeviceObjects()
    {
        DeviceObjectTests tests(DX::DeviceManager::Get().GetDevice(0));

        tests.TestShader();
        tests.TestSampler();
        tests.TestTexure1D();
        tests.TestTexure1DArray();
        tests.TestTexure2D();
        tests.TestTexure2DArray();
        tests.TestTexureCube();
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
        textureDesc.m_usage = DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        //auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        //auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTexure1DArray()
    {
        // TODO
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
        textureDesc.m_usage = DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData;

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        //auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        //auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTexure2DArray()
    {
        // TODO
    }

    void DeviceObjectTests::TestTexureCube()
    {
        // TODO
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
                    const int index = (row + col * (textureSize.x) + depth * (textureSize.x * textureSize.y)) * components;
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
        textureDesc.m_usage = DX::ResourceUsage::Immutable;
        textureDesc.m_bindFlags = DX::TextureBind_ShaderResource;
        textureDesc.m_cpuAccess = DX::ResourceCPUAccess::None;
        textureDesc.m_arrayCount = 1;
        textureDesc.m_sampleCount = 1;
        textureDesc.m_sampleQuality = 0;
        textureDesc.m_initialData = textureData.data();

        auto texture = m_device->CreateTexture(textureDesc);

        auto textureSRV = m_device->CreateShaderResourceView({ texture, texture->GetTextureDesc().m_format, 0, -1 });
        //auto textureSRWRV = m_device->CreateShaderRWResourceView({ texture, texture->GetTextureDesc().m_format, 0 });
        //auto textureRTV = m_device->CreateRenderTargetView({ texture, texture->GetTextureDesc().m_format, 0 });
    }

    void DeviceObjectTests::TestTypedBuffer()
    {
        DX_LOG(Info, "Test", " ----- Testing TypedBuffer -----");

        std::vector<float> bufferData(10);
        std::iota(bufferData.begin(), bufferData.end(), 1.0f);

        DX::BufferDesc bufferDesc = {};
        bufferDesc.m_bufferType = DX::BufferType::Typed;
        bufferDesc.m_sizeInBytes = bufferData.size() * sizeof(float);
        bufferDesc.m_usage = DX::ResourceUsage::Immutable;
        bufferDesc.m_bindFlags = DX::BufferBind_ShaderResource;
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
        //auto bufferSRWRSV = m_device->CreateShaderRWResourceView(bufferSRWRVDesc);
        //auto bufferRTV = m_device->CreateRenderTargetView(bufferRTVDesc);
    }

    void DeviceObjectTests::TestStructuredBuffer()
    {
        // TODO
    }

    void DeviceObjectTests::TestRawBuffer()
    {
        // TODO
    }
}
