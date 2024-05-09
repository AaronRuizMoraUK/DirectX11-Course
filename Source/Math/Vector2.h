#pragma once

#include "mathfu/vector.h"

namespace mathfu
{
    using Vector2 = Vector<float, 2>;

    /// @brief Calculate the dot product of two Vector2.
    ///
    /// @param v1 Vector to multiply
    /// @param v2 Vector to multiply
    /// @return Scalar dot product result.
    inline float Dot(const Vector2& v1, const Vector2& v2) {
        return Vector2::DotProduct(v1, v2);
    }

    /// @brief Normalize an Vector2.
    ///
    /// @param v1 Vector to normalize.
    /// @return Normalized vector.
    inline Vector2 Normalize(const Vector2& v1) {
        return v1.Normalized();
    }

}  // namespace mathfu
