#pragma once

#include <Math/Vector3.h>
#include <Math/Matrix3x3.h>

#include "mathfu/quaternion.h"

namespace mathfu
{
    using Quat = Quaternion<float>;

    inline Quat CreateQuatFromBasisZ(const mathfu::Vector3& basisZ, const mathfu::Vector3& up = mathfu::Vector3(0.0f, 1.0f, 0.0f))
    {
        const mathfu::Vector basisX = mathfu::Cross(up, basisZ).Normalized();
        const mathfu::Vector basisY = mathfu::Cross(basisZ, basisX);

        return mathfu::Quat::FromMatrix(CreateMatrix3x3FromBasis(basisX, basisY, basisZ));
    }
}  // namespace mathfu
