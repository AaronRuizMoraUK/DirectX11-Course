#include <Renderer/Object.h>
#include <Renderer/RendererManager.h>
#include <Assets/TextureAsset.h>
#include <Assets/MeshAsset.h>

#include <RHI/Device/Device.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Sampler/Sampler.h>

#include <Log/Log.h>
#include <Debug/Debug.h>
#include <mathfu/constants.h>

#include <algorithm>

#include <d3d11.h>

#pragma DX_DISABLE_WARNING(4267, "")

namespace DX
{
    Object::Object() = default;

    Object::~Object() = default;

    std::shared_ptr<ShaderResourceView> Object::GetDiffuseTextureView() const
    {
        return m_diffuseTextureView;
    }

    std::shared_ptr<ShaderResourceView> Object::GetEmissiveTextureView() const
    {
        return m_emissiveTextureView;
    }

    std::shared_ptr<ShaderResourceView> Object::GetNormalTextureView() const
    {
        return m_normalTextureView;
    }

    std::shared_ptr<Sampler> Object::GetSampler() const
    {
        return m_textureSampler;
    }

    std::shared_ptr<Buffer> Object::GetVertexBuffer() const
    {
        return m_vertexBuffer;
    }

    std::shared_ptr<Buffer> Object::GetIndexBuffer() const
    {
        return m_indexBuffer;
    }

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
            vertexBufferDesc.m_bufferSubType = BufferSubType::None;
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
            indexBufferDesc.m_bufferSubType = BufferSubType::None;
            indexBufferDesc.m_initialData = m_indexData.data();

            m_indexBuffer = renderer->GetDevice()->CreateBuffer(indexBufferDesc);
        }

        // Diffuse Texture
        {
            auto textureAsset = TextureAsset::LoadTextureAsset(m_diffuseFilename);
            DX_ASSERT(textureAsset.get(), "Object", "Failed to load texture");

            TextureDesc textureDesc = {};
            textureDesc.m_textureType = TextureType::Texture2D;
            textureDesc.m_dimensions = Math::Vector3Int(textureAsset->GetData()->m_size, 0);
            textureDesc.m_mipCount = 1;
            textureDesc.m_format = ResourceFormat::R8G8B8A8_UNORM;
            textureDesc.m_usage = ResourceUsage::Immutable;
            textureDesc.m_bindFlags = TextureBind_ShaderResource;
            textureDesc.m_cpuAccess = ResourceCPUAccess::None;
            textureDesc.m_arrayCount = 1;
            textureDesc.m_sampleCount = 1;
            textureDesc.m_sampleQuality = 0;
            textureDesc.m_initialData = textureAsset->GetData()->m_data;

            m_diffuseTexture = renderer->GetDevice()->CreateTexture(textureDesc);

            ShaderResourceViewDesc srvDesc = {};
            srvDesc.m_resource = m_diffuseTexture;
            srvDesc.m_viewFormat = textureDesc.m_format;
            srvDesc.m_firstMip = 0;
            srvDesc.m_mipCount = -1;

            m_diffuseTextureView = renderer->GetDevice()->CreateShaderResourceView(srvDesc);
        }

        // Emissive Texture
        {
            const ResourceFormat format = ResourceFormat::R8G8B8A8_UNORM;

            if (m_emissiveFilename.empty())
            {
                uint32_t textureData = 0; // 1 texel with all RGBA set to zero

                TextureDesc textureDesc = {};
                textureDesc.m_textureType = TextureType::Texture2D;
                textureDesc.m_dimensions = Math::Vector3Int(1, 1, 0); // 1x1 dimensions
                textureDesc.m_mipCount = 1;
                textureDesc.m_format = format;
                textureDesc.m_usage = ResourceUsage::Immutable;
                textureDesc.m_bindFlags = TextureBind_ShaderResource;
                textureDesc.m_cpuAccess = ResourceCPUAccess::None;
                textureDesc.m_arrayCount = 1;
                textureDesc.m_sampleCount = 1;
                textureDesc.m_sampleQuality = 0;
                textureDesc.m_initialData = &textureData;

                m_emissiveTexture = renderer->GetDevice()->CreateTexture(textureDesc);
            }
            else
            {
                auto textureAsset = TextureAsset::LoadTextureAsset(m_emissiveFilename);
                DX_ASSERT(textureAsset.get(), "Object", "Failed to load texture");

                TextureDesc textureDesc = {};
                textureDesc.m_textureType = TextureType::Texture2D;
                textureDesc.m_dimensions = Math::Vector3Int(textureAsset->GetData()->m_size, 0);
                textureDesc.m_mipCount = 1;
                textureDesc.m_format = format;
                textureDesc.m_usage = ResourceUsage::Immutable;
                textureDesc.m_bindFlags = TextureBind_ShaderResource;
                textureDesc.m_cpuAccess = ResourceCPUAccess::None;
                textureDesc.m_arrayCount = 1;
                textureDesc.m_sampleCount = 1;
                textureDesc.m_sampleQuality = 0;
                textureDesc.m_initialData = textureAsset->GetData()->m_data;

                m_emissiveTexture = renderer->GetDevice()->CreateTexture(textureDesc);
            }

            ShaderResourceViewDesc srvDesc = {};
            srvDesc.m_resource = m_emissiveTexture;
            srvDesc.m_viewFormat = format;
            srvDesc.m_firstMip = 0;
            srvDesc.m_mipCount = -1;

            m_emissiveTextureView = renderer->GetDevice()->CreateShaderResourceView(srvDesc);
        }

        // Normal Texture
        {
            auto textureAsset = TextureAsset::LoadTextureAsset(m_normalFilename);
            DX_ASSERT(textureAsset.get(), "Object", "Failed to load texture");

            TextureDesc textureDesc = {};
            textureDesc.m_textureType = TextureType::Texture2D;
            textureDesc.m_dimensions = Math::Vector3Int(textureAsset->GetData()->m_size, 0);
            textureDesc.m_mipCount = 1;
            textureDesc.m_format = ResourceFormat::R8G8B8A8_UNORM;
            textureDesc.m_usage = ResourceUsage::Immutable;
            textureDesc.m_bindFlags = TextureBind_ShaderResource;
            textureDesc.m_cpuAccess = ResourceCPUAccess::None;
            textureDesc.m_arrayCount = 1;
            textureDesc.m_sampleCount = 1;
            textureDesc.m_sampleQuality = 0;
            textureDesc.m_initialData = textureAsset->GetData()->m_data;

            m_normalTexture = renderer->GetDevice()->CreateTexture(textureDesc);

            ShaderResourceViewDesc srvDesc = {};
            srvDesc.m_resource = m_normalTexture;
            srvDesc.m_viewFormat = textureDesc.m_format;
            srvDesc.m_firstMip = 0;
            srvDesc.m_mipCount = -1;

            m_normalTextureView = renderer->GetDevice()->CreateShaderResourceView(srvDesc);
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

            m_textureSampler = renderer->GetDevice()->CreateSampler(samplerDesc);
        }
    }

    Cube::Cube(const Math::Transform& transform,
        const Math::Vector3& extends)
    {
        m_transform = transform;
        m_diffuseFilename = "Textures/Wall_Stone_Albedo.png";
        m_normalFilename = "Textures/Wall_Stone_Normal.png";

        const Math::Vector3 half = 0.5f * extends;

        // 6 faces, 2 triangles each face, 3 vertices each triangle.
        // Clockwise order (CW) - LeftHand

        m_vertexData =
        {
            // Front face
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 0.0f}) },

            // Back face
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 0.0f}) },

            // Right face
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 0.0f}) },

            // Left face
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector2Packed({1.0f, 0.0f}) },

            // Top face
            { Math::Vector3Packed({-half.x,  half.y, -half.z}), Math::Vector3Packed(mathfu::kAxisY3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({-half.x,  half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisY3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y,  half.z}), Math::Vector3Packed(mathfu::kAxisY3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({ half.x,  half.y, -half.z}), Math::Vector3Packed(mathfu::kAxisY3f), Math::Vector3Packed(mathfu::kAxisX3f), Math::Vector3Packed(-mathfu::kAxisZ3f), Math::Vector2Packed({1.0f, 0.0f}) },

            // Bottom face
            { Math::Vector3Packed({ half.x, -half.y,  half.z}), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector2Packed({0.0f, 0.0f}) },
            { Math::Vector3Packed({ half.x, -half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector2Packed({0.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y, -half.z}), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector2Packed({1.0f, 1.0f}) },
            { Math::Vector3Packed({-half.x, -half.y,  half.z}), Math::Vector3Packed(-mathfu::kAxisY3f), Math::Vector3Packed(-mathfu::kAxisX3f), Math::Vector3Packed(mathfu::kAxisZ3f), Math::Vector2Packed({1.0f, 0.0f}) },
        };

        // Flip UVs and calculate binormals
        for (auto& vertex : m_vertexData)
        {
            vertex.m_uv.y = -vertex.m_uv.y;
            vertex.m_binormal = Math::Vector3::CrossProduct(Math::Vector3(vertex.m_tangent), Math::Vector3(vertex.m_normal));
        }

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


    Mesh::Mesh(const Math::Transform& transform,
        const std::string& meshFilename,
        const std::string& diffuseFilename,
        const std::string& normalFilename,
        const std::string& emissiveFilename)
    {
        m_transform = transform;
        m_diffuseFilename = diffuseFilename;
        m_normalFilename = normalFilename;
        m_emissiveFilename = emissiveFilename;

        auto meshAsset = MeshAsset::LoadMeshAsset(meshFilename);
        if (!meshAsset)
        {
            DX_LOG(Fatal, "Mesh", "Failed to load mesh asset %s", meshFilename.c_str());
            return;
        }

        const MeshData* meshData = meshAsset->GetData();

        m_vertexData.resize(meshData->m_positions.size());
        for (uint32_t i = 0; i < meshData->m_positions.size(); ++i)
        {
            m_vertexData[i] = VertexPNTBUv
            {
                .m_position = meshData->m_positions[i],
                .m_normal = meshData->m_normals[i],
                .m_tangent = meshData->m_tangents[i],
                .m_binormal = meshData->m_binormals[i],
                .m_uv = meshData->m_textCoords[i]
            };
        }

        m_indexData = meshData->m_indices;

        CreateBuffers();
    }
} // namespace DX
