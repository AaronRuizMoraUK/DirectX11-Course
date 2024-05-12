#pragma once

#include <Math/Transform.h>
#include <Renderer/Vertices.h>

#include <span>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Buffer;

namespace DX
{
    class Object
    {
    public:
        Object(std::span<const Vertex> vertexData, std::span<const uint32_t> indexData);
        ~Object();

        void SetBuffers();

        uint32_t GetIndexCount() const { return m_indexCount; }

        mathfu::Transform& GetTransform() { return m_transform; }
        const mathfu::Transform& GetTransform() const { return m_transform; }
        void SetTransform(const mathfu::Transform& transform) { m_transform = transform; }

    private:
        const uint32_t m_indexCount = 0;

        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
        ComPtr<ID3D11Buffer> m_worldMatrixConstantBuffer;

        mathfu::Transform m_transform = mathfu::Transform::CreateIdentity();

        mathfu::Matrix4x4Packed m_worldMatrix = mathfu::Matrix4x4::Identity();
    };
} // namespace DX
