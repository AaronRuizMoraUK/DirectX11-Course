#include <Renderer/Object.h>
#include <Renderer/RendererManager.h>
#include <Assets/TextureAsset.h>

#include <RHI/Device/Device.h>
#include <RHI/Device/DeviceContext.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Sampler/Sampler.h>
#include <RHI/Pipeline/PipelineResourceBindings.h>

#include <File/FileUtils.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    Object::Object() = default;

    Object::~Object() = default;

    void Object::CreateBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer();
        DX_ASSERT(renderer, "Object", "Default renderer not found");

        // Vertex Buffer
        {
            BufferDesc vertexBufferDesc = {};
            vertexBufferDesc.m_elementSizeInBytes = GetVertexSize();
            vertexBufferDesc.m_elementCount = m_vertexData.size();
            vertexBufferDesc.m_usage = ResourceUsage::Immutable;
            vertexBufferDesc.m_bindFlags = BufferBind_VertexBuffer;
            vertexBufferDesc.m_cpuAccess = ResourceCPUAccess::None;
            vertexBufferDesc.m_bufferType = BufferType::None;
            vertexBufferDesc.m_initialData = m_vertexData.data();

            m_vertexBuffer = renderer->GetDevice()->CreateBuffer(vertexBufferDesc);
        }

        // Index Buffer
        {
            BufferDesc indexBufferDesc = {};
            indexBufferDesc.m_elementSizeInBytes = GetIndexSize();
            indexBufferDesc.m_elementCount = m_indexData.size();
            indexBufferDesc.m_usage = ResourceUsage::Immutable;
            indexBufferDesc.m_bindFlags = BufferBind_IndexBuffer;
            indexBufferDesc.m_cpuAccess = ResourceCPUAccess::None;
            indexBufferDesc.m_bufferType = BufferType::None;
            indexBufferDesc.m_initialData = m_indexData.data();

            m_indexBuffer = renderer->GetDevice()->CreateBuffer(indexBufferDesc);
        }

        // Constant Buffer
        {
            const Math::Matrix4x4Packed worldMatrix = m_transform.ToMatrix();

            BufferDesc constantBufferDesc = {};
            constantBufferDesc.m_elementSizeInBytes = sizeof(Math::Matrix4x4Packed);
            constantBufferDesc.m_elementCount = 1;
            constantBufferDesc.m_usage = ResourceUsage::Dynamic;
            constantBufferDesc.m_bindFlags = BufferBind_ConstantBuffer;
            constantBufferDesc.m_cpuAccess = ResourceCPUAccess::Write;
            constantBufferDesc.m_bufferType = BufferType::None;
            constantBufferDesc.m_initialData = &worldMatrix;

            m_worldMatrixConstantBuffer = renderer->GetDevice()->CreateBuffer(constantBufferDesc);
        }

        // Texture
        {
            m_textureAsset = TextureAsset::LoadTextureAsset("Textures/Wall_Stone_Albedo.png");
            DX_ASSERT(m_textureAsset.get(), "Object", "Failed to load texture");

            TextureDesc textureDesc = {};
            textureDesc.m_textureType = TextureType::Texture2D;
            textureDesc.m_dimensions = Math::Vector3Int(m_textureAsset->GetData()->m_size, 0);
            textureDesc.m_mipCount = 1;
            textureDesc.m_format = ResourceFormat::R8G8B8A8_UNORM;
            textureDesc.m_usage = ResourceUsage::Immutable;
            textureDesc.m_bindFlags = TextureBind_ShaderResource;
            textureDesc.m_cpuAccess = ResourceCPUAccess::None;
            textureDesc.m_arrayCount = 1;
            textureDesc.m_sampleCount = 1;
            textureDesc.m_sampleQuality = 0;
            textureDesc.m_initialData = m_textureAsset->GetData()->m_data;

            m_texture = renderer->GetDevice()->CreateTexture(textureDesc);

            ShaderResourceViewDesc srvDesc = {};
            srvDesc.m_resource = m_texture;
            srvDesc.m_viewFormat = textureDesc.m_format;
            srvDesc.m_firstMip = 0;
            srvDesc.m_mipCount = -1;

            m_textureView = renderer->GetDevice()->CreateShaderResourceView(srvDesc);
        }

        // Sampler State
        {
            SamplerDesc samplerDesc = {};
            samplerDesc.m_minFilter = FilterSampling::Linear;
            samplerDesc.m_magFilter = FilterSampling::Linear;
            samplerDesc.m_mipFilter = FilterSampling::Linear;
            samplerDesc.m_filterMode = FilterMode::Normal;
            samplerDesc.m_addressU = AddressMode::Wrap;
            samplerDesc.m_addressV = AddressMode::Wrap;
            samplerDesc.m_addressW = AddressMode::Wrap;
            samplerDesc.m_mipBias = 0;
            samplerDesc.m_mipClamp = NoMipClamping;
            samplerDesc.m_maxAnisotropy = 1;
            samplerDesc.m_borderColor = Math::Color(0.0f);
            samplerDesc.m_comparisonFunction = ComparisonFunction::Always;

            m_sampler = renderer->GetDevice()->CreateSampler(samplerDesc);
        }
    }

    void Object::SetBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer();
        DX_ASSERT(renderer, "Object", "Default renderer not found");

        // Update constant buffer with the latest world matrix.
        {
            const Math::Matrix4x4Packed worldMatrix = m_transform.ToMatrix();

            renderer->GetDevice()->GetImmediateContext().UpdateDynamicBuffer(*m_worldMatrixConstantBuffer, &worldMatrix, sizeof(worldMatrix));
        }

        renderer->GetDevice()->GetImmediateContext().BindVertexBuffers({m_vertexBuffer.get()});
        renderer->GetDevice()->GetImmediateContext().BindIndexBuffer(*m_indexBuffer);

        renderer->GetPipelineResources().SetConstantBuffer(ShaderType_Vertex, 1, m_worldMatrixConstantBuffer);
        renderer->GetPipelineResources().SetShaderResourceView(ShaderType_Pixel, 0, m_textureView);
        renderer->GetPipelineResources().SetSampler(ShaderType_Pixel, 0, m_sampler);
    }

    Triangle::Triangle()
    {
        // Clockwise order (CW) - LeftHand

        m_vertexData =
        {
            { Math::Vector3Packed({-0.5f, -0.5f, 0.0f}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({0.0f,   0.5f, 0.0f}), Math::Vector2Packed({0.5f, 1.0f}) },
            { Math::Vector3Packed({0.5f,  -0.5f, 0.0f}), Math::Vector2Packed({1.0f, 0.0f}) }
        };

        m_indexData = { 0, 1, 2 };

        CreateBuffers();
    }

    Cube::Cube(const Math::Vector3& extends)
    {
        const Math::Vector3 half = 0.5f * extends;

        // 6 faces, 2 triangles each face, 3 vertices each triangle.
        // Clockwise order (CW) - LeftHand

        m_vertexData =
        {
            // Front face
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector2Packed({1.0f, 0.0f}) },

            // Back face
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector2Packed({1.0f, 0.0f}) },

            // Right face
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector2Packed({1.0f, 0.0f}) },

            // Left face
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector2Packed({1.0f, 0.0f}) },

            // Top face
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector2Packed({1.0f, 0.0f}) },

            // Bottom face
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector2Packed({1.0f, 0.0f}) },
        };

        m_indexData =
        {
            // Front face
            0, 1, 2,
            2, 3, 0,

            // Back face
            4, 5, 6,
            6, 7, 4,

            // Right face
            8, 9, 10,
            10, 11, 8,

            // Left face
            12, 13, 14,
            14, 15, 12,

            // Top face
            16, 17, 18,
            18, 19, 16,

            // Top face
            20, 23, 22,
            22, 21, 20
        };

        CreateBuffers();
    }
} // namespace DX
