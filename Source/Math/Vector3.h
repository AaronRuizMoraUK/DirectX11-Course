#pragma once

#include "mathfu/vector.h"

namespace mathfu
{
    using Vector3 = Vector<float, 3>;

    /// @brief Calculate the cross product of two Vector3.
    ///
    /// @param v1 Vector to multiply
    /// @param v2 Vector to multiply
    /// @return 3-dimensional vector that contains the result.
    inline Vector3 Cross(const Vector3& v1, const Vector3& v2) {
        return Vector3::CrossProduct(v1, v2);
    }

    /// @brief Calculate the dot product of two Vector3.
    ///
    /// @param v1 Vector to multiply
    /// @param v2 Vector to multiply
    /// @return Scalar dot product result.
    inline float Dot(const Vector3& v1, const Vector3& v2) {
        return Vector3::DotProduct(v1, v2);
    }

    /// @brief Normalize an Vector3.
    ///
    /// @param v1 Vector to normalize.
    /// @return Normalized vector.
    inline Vector3 Normalize(const Vector3& v1) {
        return v1.Normalized();
    }

}  // namespace mathfu
