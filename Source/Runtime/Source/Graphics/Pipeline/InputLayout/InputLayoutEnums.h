#pragma once

namespace DX
{
    enum class InputSemantic
    {
        Unknown = 0,

        Position,
        Normal,
        Tangent,
        Binormal,
        TexCoord,
        Color,
        BlendIndices,
        BlendWeight,

        CustomName,

        Count
    };

    enum class PrimitiveTopology
    {
        Undefined = 0,
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
