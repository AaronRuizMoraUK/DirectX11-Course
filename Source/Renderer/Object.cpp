#include <Renderer/Object.h>
#include <Renderer/RendererManager.h>

#include <d3d11.h>

namespace DX
{
    Object::Object() = default;

    Object::~Object() = default;

    void Object::CreateBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        assert(renderer);

        // Vertex Buffer
        {
            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = GetVertexSize() * m_vertexData.size();
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
            vertexSubresourceData.pSysMem = m_vertexData.data();
            vertexSubresourceData.SysMemPitch = 0;
            vertexSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, m_vertexBuffer.GetAddressOf());
        }

        // Index Buffer
        {
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.ByteWidth = GetIndexxSize() * m_indexData.size();
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
            indexSubresourceData.pSysMem = m_indexData.data();
            indexSubresourceData.SysMemPitch = 0;
            indexSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, m_indexBuffer.GetAddressOf());
        }

        // Constant Buffer
        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            constantBufferDesc.ByteWidth = sizeof(mathfu::Matrix4x4Packed);
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
            constantBufferDesc.MiscFlags = 0;

            const mathfu::Matrix4x4Packed worldMatrix = m_transform.ToMatrix();

            D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
            constantSubresourceData.pSysMem = &worldMatrix;
            constantSubresourceData.SysMemPitch = 0;
            constantSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, m_worldMatrixConstantBuffer.GetAddressOf());
        }
    }

    void Object::SetBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        assert(renderer);

        // Update constant buffer with the latest world matrix.
        {
            const mathfu::Matrix4x4Packed worldMatrix = m_transform.ToMatrix();

            D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
            renderer->GetDeviceContext()->Map(m_worldMatrixConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
            memcpy(mappedSubresource.pData, &worldMatrix, sizeof(mathfu::Matrix4x4Packed));
            renderer->GetDeviceContext()->Unmap(m_worldMatrixConstantBuffer.Get(), 0);
        }

        const uint32_t vertexBufferStride = GetVertexSize();
        const uint32_t vertexBufferOffset = 0;

        renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
        renderer->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        renderer->GetDeviceContext()->VSSetConstantBuffers(1, 1, m_worldMatrixConstantBuffer.GetAddressOf());
    }

    Triangle::Triangle()
    {
        // Clockwise order (CW) - LeftHand

        m_vertexData =
        {{
            { mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Vector2Packed(mathfu::Vector2(0.5f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) }
        }};

        m_indexData = { 0, 1, 2 };

        CreateBuffers();
    }

    Cube::Cube(const mathfu::Vector3& extends)
    {
        const mathfu::Vector3 half = 0.5f * extends;

        // 6 faces, 2 triangles each face, 3 vertices each triangle.
        // Clockwise order (CW) - LeftHand

        m_vertexData =
        {{
            // Front face
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },

            // Back face
            { mathfu::Vector3Packed(mathfu::Vector3( half.x, -half.y, half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x,  half.y, half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y, half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },

            // Right face
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x,  half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },

            // Left face
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },

            // Top face
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x,  half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x,  half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x,  half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },

            // Bottom face
            { mathfu::Vector3Packed(mathfu::Vector3( half.x, -half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 0.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3( half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(0.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, -half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 1.0f)) },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y,  half.z)), mathfu::Vector2Packed(mathfu::Vector2(1.0f, 0.0f)) },
        }};

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
