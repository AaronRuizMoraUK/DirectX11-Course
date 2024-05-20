#pragma once

#include <Math/Transform.h>
#include <Renderer/Vertices.h>

#include <vector>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Buffer;
class ID3D11Texture2D;
class ID3D11ShaderResourceView;
class ID3D11SamplerState;

namespace DX
{
    class Object
    {
    public:
        Object();
        virtual ~Object();

        void SetBuffers();

        uint32_t GetIndexCount() const { return m_indexData.size(); }

        Math::Transform& GetTransform() { return m_transform; }
        const Math::Transform& GetTransform() const { return m_transform; }
        void SetTransform(const Math::Transform& transform) { m_transform = transform; }

    protected:
        void CreateBuffers();

        uint32_t GetVertexSize() const { return sizeof(VertexPUV); }
        uint32_t GetIndexxSize() const { return sizeof(Index); }

        Math::Transform m_transform = Math::Transform::CreateIdentity();

        std::vector<VertexPUV> m_vertexData;
        std::vector<Index> m_indexData;

    private:
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        ComPtr<ID3D11Buffer> m_worldMatrixConstantBuffer;

        uint8_t* m_textureData = nullptr;
        Math::Vector2Int m_textureSize = Math::Vector2Int(0);

        ComPtr<ID3D11Texture2D> m_texture;
        ComPtr<ID3D11ShaderResourceView> m_textureView;
        ComPtr<ID3D11SamplerState> m_samplerState;
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
} // namespace DX
