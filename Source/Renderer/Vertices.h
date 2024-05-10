#pragma once

#include <Math/Vector3.h>
#include <Math/Color.h>

namespace DX
{
    struct Vertex
    {
        mathfu::Vector3Packed position;
        mathfu::ColorPacked color;
    };
} // namespace DX
