#pragma once

#include <Math/Vector3.h>
#include <Math/Color.h>

#include <span>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Buffer;

namespace DX
{
    struct Vertex
    {
        mathfu::Vector3Packed position;
        mathfu::ColorPacked color;
    };

    class Object
    {
    public:
        Object(std::span<const Vertex> vertexData, std::span<const uint32_t> indexData);
        ~Object();

        void SetBuffers();

        uint32_t GetIndexCount() const { return m_indexCount; }

    private:
        const uint32_t m_indexCount = 0;

        ComPtr<ID3D11Buffer> m_vertexBuffer;
        ComPtr<ID3D11Buffer> m_indexBuffer;
    };
} // namespace DX
