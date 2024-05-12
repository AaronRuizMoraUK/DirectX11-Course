#include <Renderer/Object.h>
#include <Renderer/RendererManager.h>

#include <d3d11.h>

namespace DX
{
    Object::Object(std::span<const VertexPC> vertexData, std::span<const uint32_t> indexData)
        : m_indexCount(indexData.size())
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        assert(renderer);

        {
            D3D11_BUFFER_DESC vertexBufferDesc = {};
            vertexBufferDesc.ByteWidth = sizeof(VertexPC) * vertexData.size();
            vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            vertexBufferDesc.CPUAccessFlags = 0;
            vertexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
            vertexSubresourceData.pSysMem = vertexData.data();
            vertexSubresourceData.SysMemPitch = 0;
            vertexSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, m_vertexBuffer.GetAddressOf());
        }

        {
            D3D11_BUFFER_DESC indexBufferDesc = {};
            indexBufferDesc.ByteWidth = sizeof(uint32_t) * indexData.size();
            indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
            indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            indexBufferDesc.CPUAccessFlags = 0;
            indexBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
            indexSubresourceData.pSysMem = indexData.data();
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

            D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
            constantSubresourceData.pSysMem = &m_worldMatrix;
            constantSubresourceData.SysMemPitch = 0;
            constantSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, m_worldMatrixConstantBuffer.GetAddressOf());
        }
    }

    Object::~Object() = default;

    void Object::SetBuffers()
    {
        const uint32_t vertexBufferStride = sizeof(VertexPC);
        const uint32_t vertexBufferOffset = 0;

        auto* renderer = RendererManager::Get().GetRenderer(0);
        assert(renderer);

        m_worldMatrix = m_transform.ToMatrix();

        // Update constant buffer with the latest world matrix.
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
            renderer->GetDeviceContext()->Map(m_worldMatrixConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
            memcpy(mappedSubresource.pData, &m_worldMatrix, sizeof(mathfu::Matrix4x4Packed));
            renderer->GetDeviceContext()->Unmap(m_worldMatrixConstantBuffer.Get(), 0);
        }

        renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
        renderer->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        renderer->GetDeviceContext()->VSSetConstantBuffers(1, 1, m_worldMatrixConstantBuffer.GetAddressOf());
    }
} // namespace DX
