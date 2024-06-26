#pragma once

#include <Math/Vector2.h>

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
        Unknown = 0,

        Normal,
        Minimum, // Returns the minimum of the sampled values during filtering.
        Maximum, // Returns the maximum of the sampled values during filtering.

        // When sampling the texture in the shader it'll compare the texture's
        // value with another value passed as parameter.
        // It'll return 1.0 if the comparison is true or 0.0 otherwise.
        // The comparison function is set in the sampler state.
        Comparison,

        Count
    };

    // How to sample with texture coordinate outside [0,1]
    enum class AddressMode
    {
        Unknown = 0,

        Wrap,
        Mirror,
        Clamp,
        BorderColor, // Color specified in the sampler state.
        MirrorOnce,

        Count
    };

    static const float MaxMipLevel = 3.402823466e+38f;
    static const Math::Vector2 NoMipClamping = Math::Vector2(0.0f, MaxMipLevel);
} // namespace DX
