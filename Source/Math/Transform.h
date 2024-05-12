#pragma once

#include <Math/Vector3.h>
#include <Math/Quat.h>
#include <Math/Matrix4x4.h>

namespace mathfu
{
    class Transform
    {
    public:
        static Transform CreateIdentity();
        static Transform CreateFromMatrix(const mathfu::Matrix4x4& matrix);

        Transform() = default; // Does not initialize members

        Transform(
            const mathfu::Vector3& position, 
            const mathfu::Quat& rotation = mathfu::Quat::identity, 
            const mathfu::Vector3& scale = mathfu::Vector3(1.0f));

        mathfu::Vector3 GetBasisX() const;
        mathfu::Vector3 GetBasisY() const;
        mathfu::Vector3 GetBasisZ() const;

        mathfu::Matrix4x4 ToMatrix() const;

        mathfu::Vector3 m_position;
        mathfu::Quat m_rotation;
        mathfu::Vector3 m_scale;
    };
} // namespace mathfu
