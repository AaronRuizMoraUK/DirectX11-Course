#pragma once

namespace DX
{
    enum class PrimitiveTopology
    {
        Undefined,
        PointList,
        LineList,
        LineStrip,
        TriangleList,
        TriangleStrip,
        LineListAdjacency,
        LineStripAdjacency,
        TriangleListAdjacency,
        TriangleStripAdjacency,
        ControlPointPatchList,

        Count
    };
} // namespace DX
