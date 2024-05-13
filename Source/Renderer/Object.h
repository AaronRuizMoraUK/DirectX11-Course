#pragma once

#include <Math/Transform.h>
#include <Renderer/Vertices.h>

#include <vector>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Buffer;

namespace DX
{
    class Object
    {
    public:
        Object();
        virtual ~Object();

        void SetBuffers();

        uint32_t GetIndexCount() const { return m_indexData.size(); }

        mathfu::Transform& GetTransform() { return m_transform; }
        const mathfu::Transform& GetTransform() const { return m_transform; }
        void SetTransform(const mathfu::Transform& transform) { m_transform = transform; }

    protected:
        void CreateBuffers();

        uint32_t GetVertexSize() const { return sizeof(VertexPUV); }
        uint32_t GetIndexxSize() const { return sizeof(Index); }

        mathfu::Transform m_transform = mathfu::Transform::CreateIdentity();

        std::vector<VertexPUV> m_vertexData;
        std::vector<Index> m_indexData;

    private:
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        ComPtr<ID3D11Buffer> m_worldMatrixConstantBuffer;
    };

    class Triangle : public Object
    {
    public:
        Triangle();
    };

    class Cube : public Object
    {
    public:
        Cube(const mathfu::Vector3& extends);
    };
} // namespace DX
