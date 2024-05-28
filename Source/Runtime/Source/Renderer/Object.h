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

        std::shared_ptr<ShaderResourceView> GetTextureView() const;
        std::shared_ptr<Sampler> GetSampler() const;

        std::shared_ptr<Buffer> GetVertexBuffer() const;
        std::shared_ptr<Buffer> GetIndexBuffer() const;

    protected:
        void CreateBuffers();

        uint32_t GetVertexSize() const { return sizeof(VertexPNTBUv); }
        uint32_t GetIndexSize() const { return sizeof(Index); }

        Math::Transform m_transform = Math::Transform::CreateIdentity();

        std::vector<VertexPNTBUv> m_vertexData;
        std::vector<Index> m_indexData;

    private:
        std::shared_ptr<Buffer> m_vertexBuffer;
        std::shared_ptr<Buffer> m_indexBuffer;

        std::shared_ptr<Texture> m_texture;
        std::shared_ptr<ShaderResourceView> m_textureView;
        std::shared_ptr<Sampler> m_sampler;
    };

    class Cube : public Object
    {
    public:
        Cube(const Math::Vector3& extends);
    };

    class Mesh : public Object
    {
    public:
        Mesh(const std::string& filename);
    };
} // namespace DX
