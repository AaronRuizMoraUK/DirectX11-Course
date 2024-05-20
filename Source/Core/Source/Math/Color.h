#pragma once

#include "mathfu/vector.h"

namespace Math
{
    // Color size might differ depending on platform and build configuration of mathfu (SIMD, padding, etc).
    // To serialize and deserialize as a flat array, use ColorPacked, which is a POD version of Color.
    using Color = mathfu::Vector<float, 4>;
    using ColorPacked = mathfu::VectorPacked<float, 4>;

    namespace Colors
    {
        static const Color Black = Color(0.0f, 0.0f, 0.0f, 1.0f);
        static const Color White = Color(1.0f, 1.0f, 1.0f, 1.0f);
        static const Color Red = Color(1.0f, 0.0f, 0.0f, 1.0f);
        static const Color Green = Color(0.0f, 1.0f, 0.0f, 1.0f);
        static const Color Blue = Color(0.0f, 0.0f, 1.0f, 1.0f);
        static const Color Yellow = Color(1.0f, 1.0f, 0.0f, 1.0f);
        static const Color Cyan = Color(0.0f, 1.0f, 1.0f, 1.0f);
        static const Color Magenta = Color(1.0f, 0.0f, 1.0f, 1.0f);
    }

    // Create color passing float values in the range [0.0f, 1.0f]
    inline Color CreateColor(float r, float g, float b, float a)
    {
        return Color(r, g, b, a);
    }

    // Create color passing byte values in the range [0, 255]
    inline Color CreateColor(std::byte r, std::byte g, std::byte b, std::byte a)
    {
        return Color(static_cast<float>(r), static_cast<float>(g),
                     static_cast<float>(b), static_cast<float>(a)) / 255.0f;
    }
}  // namespace Math
