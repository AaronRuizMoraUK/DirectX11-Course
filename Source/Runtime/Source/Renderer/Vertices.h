#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Color.h>

namespace DX
{
    using Index = uint32_t;

    struct VertexPC
    {
        Math::Vector3Packed m_position;
        Math::ColorPacked m_color;
    };

    struct VertexPUV
    {
        Math::Vector3Packed m_position;
        Math::Vector2Packed m_uv;
    };
} // namespace DX
