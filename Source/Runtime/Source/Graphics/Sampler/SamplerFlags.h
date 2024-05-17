#pragma once

namespace DX
{
    enum class FilterSampling
    {
        Point,
        Linear,
        Anisotropic,

        Count
    };

    enum class FilterMode
    {
        Normal,
        Minimum,
        Maximum,
        Comparison,

        Count
    };

    enum class AddressMode
    {
        Wrap,
        Mirror,
        Clamp,
        BorderColor,
        MirrorOnce,

        Count
    };
} // namespace DX
