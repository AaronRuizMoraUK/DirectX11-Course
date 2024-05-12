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

        {
            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = sizeof(VertexPC) * m_vertexData.size();
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

        {
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.ByteWidth = sizeof(Index) * m_indexData.size();
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
        const uint32_t vertexBufferStride = sizeof(VertexPC);
        const uint32_t vertexBufferOffset = 0;

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

        renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
        renderer->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        renderer->GetDeviceContext()->VSSetConstantBuffers(1, 1, m_worldMatrixConstantBuffer.GetAddressOf());
    }

    Triangle::Triangle()
    {
        m_vertexData =
        {{
            { mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Colors::RedPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Colors::GreenPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Colors::BluePacked }
        }};

        m_indexData = { 0, 1, 2 };

        CreateBuffers();
    }

    Cube::Cube(const mathfu::Vector3& extends)
    {
        const mathfu::Vector3 half = 0.5f * extends;

        m_vertexData =
        {{
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, -half.z)), mathfu::Colors::RedPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, -half.y, half.z)), mathfu::Colors::GreenPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y, half.z)), mathfu::Colors::BluePacked },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, -half.y, -half.z)), mathfu::Colors::CyanPacked },

            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, half.y, -half.z)), mathfu::Colors::MagentaPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(-half.x, half.y, half.z)), mathfu::Colors::YellowPacked },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, half.y, half.z)), mathfu::Colors::WhitePacked },
            { mathfu::Vector3Packed(mathfu::Vector3(half.x, half.y, -half.z)), mathfu::Colors::BlackPacked },
        }};

        m_indexData = // 6 faces, 2 triangles each face, 3 vertices each triangle.
        {
            2, 1, 0,
            0, 3, 2,

            4, 5, 6,
            6, 7, 4,

            1, 2, 6,
            6, 5, 1,

            2, 3, 7,
            7, 6, 2,

            3, 0, 4,
            4, 7, 3,

            0, 1, 5,
            5, 4, 0
        };

        CreateBuffers();
    }
} // namespace DX
