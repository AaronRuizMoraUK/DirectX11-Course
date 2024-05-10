#include <Window/WindowManager.h>
#include <Renderer/RendererManager.h>
#include <Math/Vector3.h>
#include <Math/Color.h>

#include <cstdio>
#include <array>
#include <memory>

#include <d3d11.h>

struct Vertex
{
    mathfu::Vector3Packed position;
    mathfu::ColorPacked color;
};

static const std::array<Vertex, 3> VertexData = 
{
    Vertex(mathfu::Vector3Packed(mathfu::Vector3(-0.5f, -0.5f, 0.0f)), mathfu::Colors::RedPacked),
    Vertex(mathfu::Vector3Packed(mathfu::Vector3(0.0f,   0.5f, 0.0f)), mathfu::Colors::GreenPacked),
    Vertex(mathfu::Vector3Packed(mathfu::Vector3(0.5f,  -0.5f, 0.0f)), mathfu::Colors::BluePacked)
};

static const std::array<uint32_t, 3> IndexData = { 0, 1, 2 };

void SetTriangle(DX::Renderer* renderer)
{
    ComPtr<ID3D11Buffer> vertexBuffer;
    {
        D3D11_BUFFER_DESC vertexBufferDesc = {};
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * VertexData.size();
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
        vertexSubresourceData.pSysMem = VertexData.data();
        vertexSubresourceData.SysMemPitch = 0;
        vertexSubresourceData.SysMemSlicePitch = 0;

        renderer->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubresourceData, &vertexBuffer);
    }

    ComPtr<ID3D11Buffer> indexBuffer;
    {
        D3D11_BUFFER_DESC indexBufferDesc = {};
        indexBufferDesc.ByteWidth = sizeof(uint32_t) * IndexData.size();
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
        indexSubresourceData.pSysMem = IndexData.data();
        indexSubresourceData.SysMemPitch = 0;
        indexSubresourceData.SysMemSlicePitch = 0;

        renderer->GetDevice()->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
    }

    const uint32_t vertexBufferStride = sizeof(Vertex);
    const uint32_t vertexBufferOoffset = 0;

    renderer->GetDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &vertexBufferStride, &vertexBufferOoffset);
    renderer->GetDeviceContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

int main()
{
    DX::WindowManager& windowManager = DX::WindowManager::Get();
    DX::Window* window = windowManager.CreateWindowWithTitle(DX::WindowSize{1280, 720}, "DirectX11 Course");
    if (!window)
    {
        return -1;
    }

    DX::RendererManager& rendererManager = DX::RendererManager::Get();
    DX::Renderer* renderer = rendererManager.CreateRenderer(window);
    if (!renderer)
    {
        return -1;
    }

    SetTriangle(renderer);

    while (window->IsVisible())
    {
        windowManager.PollEvents();

        renderer->ClearColor(mathfu::Colors::Black);

        renderer->SetPipeline();

        renderer->Draw(IndexData.size());

        renderer->Present();
    }

    DX::RendererManager::Destroy();
    DX::WindowManager::Destroy();

    std::printf("Done!\n");
    return 0;
}
