#pragma once

namespace DX
{
    enum class ShaderType
    {
        Unknown = 0,

        Vertex,
        Hull,
        Domain,
        Geometry,
        Pixel,
        Compute,

        Count
    };
} // namespace DX
