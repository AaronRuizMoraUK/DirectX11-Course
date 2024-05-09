#pragma once

#include "mathfu/vector.h"

namespace mathfu
{
    using Vector4 = Vector<float, 4>;

    /// @brief Calculate the dot product of two Vector4.
    ///
    /// @param v1 Vector to multiply
    /// @param v2 Vector to multiply
    /// @return Scalar dot product result.
    inline float Dot(const Vector4& v1, const Vector4& v2) {
        return Vector4::DotProduct(v1, v2);
    }

    /// @brief Normalize an Vector2.
    ///
    /// @param v1 Vector to normalize.
    /// @return Normalized vector.
    inline Vector4 Normalize(const Vector4& v1) {
        return v1.Normalized();
    }

}  // namespace mathfu
