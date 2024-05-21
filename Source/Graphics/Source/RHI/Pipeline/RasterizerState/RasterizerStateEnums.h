#pragma once

namespace DX
{
    enum class FaceFillMode
    {
        Unknown = 0,

        Wireframe = 1,
        Solid = 2,

        Count,
    };

    enum class FaceFrontOrder
    {
        Unknown = 0,

        Clockwise = 1,
        CounterClockwise = 2,

        Count,
    };

    enum class FaceCullMode
    {
        None = 0,
        FrontFace = 1,
        BackFace = 2,

        Count,
    };
} // namespace DX
