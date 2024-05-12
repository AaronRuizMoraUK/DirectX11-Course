#pragma once

#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Color.h>

namespace DX
{
    struct VertexPC
    {
        mathfu::Vector3Packed m_position;
        mathfu::ColorPacked m_color;
    };

    struct VertexPUV
    {
        mathfu::Vector3Packed m_position;
        mathfu::Vector2Packed m_uv;
    };
} // namespace DX
