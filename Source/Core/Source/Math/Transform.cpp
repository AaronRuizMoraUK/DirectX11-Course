#include <Math/Transform.h>

#include <mathfu/constants.h>

namespace mathfu
{
    Transform Transform::CreateIdentity()
    {
        return Transform(mathfu::Vector3(0.0f));
    }

    Transform Transform::CreateFromMatrix(const mathfu::Matrix4x4& matrix)
    {
        return Transform(
            matrix.TranslationVector3D(),
            mathfu::Quat::FromMatrix(matrix),
            matrix.ScaleVector3D());
    }

    Transform::Transform(const mathfu::Vector3& position, const mathfu::Quat& rotation, const mathfu::Vector3& scale)
        : m_position(position)
        , m_rotation(rotation)
        , m_scale(scale)
    {
    }

    mathfu::Vector3 Transform::GetBasisX() const
    {
        return m_rotation * (mathfu::kAxisX3f * m_scale);
    }

    mathfu::Vector3 Transform::GetBasisY() const
    {
        return m_rotation * (mathfu::kAxisY3f * m_scale);
    }

    mathfu::Vector3 Transform::GetBasisZ() const
    {
        return m_rotation * (mathfu::kAxisZ3f * m_scale);
    }

    mathfu::Matrix4x4 Transform::ToMatrix() const
    {
        return mathfu::Matrix4x4::Transform(m_position, m_rotation.ToMatrix(), m_scale);
    }
} // namespace mathfu
