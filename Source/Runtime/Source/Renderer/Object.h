#pragma once

#include <Math/Transform.h>
#include <Renderer/Vertices.h>

#include <vector>
#include <memory>
#include <string>

namespace DX
{
    class Buffer;
    class Texture;
    class ShaderResourceView;
    class Sampler;
    class CommandList;
    class PipelineResourceBindings;

    class Object
    {
    public:
        Object();
        virtual ~Object();

        uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_indexData.size()); }

        Math::Transform& GetTransform() { return m_transform; }
        const Math::Transform& GetTransform() const { return m_transform; }
        void SetTransform(const Math::Transform& transform) { m_transform = transform; }

        std::shared_ptr<ShaderResourceView> GetDiffuseTextureView() const;
        std::shared_ptr<ShaderResourceView> GetEmissiveTextureView() const;
        std::shared_ptr<ShaderResourceView> GetNormalTextureView() const;
        std::shared_ptr<Sampler> GetSampler() const;

        std::shared_ptr<Buffer> GetVertexBuffer() const;
        std::shared_ptr<Buffer> GetIndexBuffer() const;

    protected:
        void CreateBuffers();

        uint32_t GetVertexSize() const { return sizeof(VertexPNTBUv); }
        uint32_t GetIndexSize() const { return sizeof(Index); }

        Math::Transform m_transform = Math::Transform::CreateIdentity();

        // Filled by subclass
        std::vector<VertexPNTBUv> m_vertexData;
        std::vector<Index> m_indexData;

        // Filled by subclass
        std::string m_diffuseFilename;
        std::string m_emissiveFilename;
        std::string m_normalFilename;

    private:
        std::shared_ptr<Buffer> m_vertexBuffer;
        std::shared_ptr<Buffer> m_indexBuffer;

        std::shared_ptr<Texture> m_diffuseTexture;
        std::shared_ptr<Texture> m_emissiveTexture;
        std::shared_ptr<Texture> m_normalTexture;
        std::shared_ptr<ShaderResourceView> m_diffuseTextureView;
        std::shared_ptr<ShaderResourceView> m_emissiveTextureView;
        std::shared_ptr<ShaderResourceView> m_normalTextureView;
        std::shared_ptr<Sampler> m_textureSampler;
    };

    class Cube : public Object
    {
    public:
        Cube(const Math::Transform& transform, 
            const Math::Vector3& extends);
    };

    class Mesh : public Object
    {
    public:
        Mesh(const Math::Transform& transform,
            const std::string& meshFilename, 
            const std::string& diffuseFilename, 
            const std::string& normalFilename,
            const std::string& emissiveFilename = "");
    };
} // namespace DX
