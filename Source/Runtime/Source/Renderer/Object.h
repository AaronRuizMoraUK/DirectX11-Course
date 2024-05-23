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
    class TextureAsset;

    class Object
    {
    public:
        Object();
        virtual ~Object();

        void SetBuffers();

        uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_indexData.size()); }

        Math::Transform& GetTransform() { return m_transform; }
        const Math::Transform& GetTransform() const { return m_transform; }
        void SetTransform(const Math::Transform& transform) { m_transform = transform; }

    protected:
        void CreateBuffers();

        uint32_t GetVertexSize() const { return sizeof(VertexPUV); }
        uint32_t GetIndexSize() const { return sizeof(Index); }

        Math::Transform m_transform = Math::Transform::CreateIdentity();

        std::vector<VertexPUV> m_vertexData;
        std::vector<Index> m_indexData;

    private:
        std::shared_ptr<Buffer> m_vertexBuffer;
        std::shared_ptr<Buffer> m_indexBuffer;
        std::shared_ptr<Buffer> m_worldMatrixConstantBuffer;

        std::shared_ptr<TextureAsset> m_textureAsset;
        Math::Vector2Int m_textureSize = Math::Vector2Int(0);

        std::shared_ptr<Texture> m_texture;
        std::shared_ptr<ShaderResourceView> m_textureView;
        std::shared_ptr<Sampler> m_sampler;
    };

    class Triangle : public Object
    {
    public:
        Triangle();
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
