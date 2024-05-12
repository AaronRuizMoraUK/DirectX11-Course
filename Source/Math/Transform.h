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

        inline mathfu::Vector3 GetPosition() const;
        inline mathfu::Quat GetRotation() const;
        inline mathfu::Vector3 GetScale() const;

        inline void SetPosition(const mathfu::Vector3& position);
        inline void SetRotation(const mathfu::Quat& rotation);
        inline void SetScale(const mathfu::Vector3& scale);

        mathfu::Vector3 GetBasisX() const;
        mathfu::Vector3 GetBasisY() const;
        mathfu::Vector3 GetBasisZ() const;

        mathfu::Matrix4x4 ToMatrix() const;

    private:
        mathfu::Vector3 m_position;
        mathfu::Quat m_rotation;
        mathfu::Vector3 m_scale;
    };

    mathfu::Vector3 Transform::GetPosition() const
    {
        return m_position;
    }

    mathfu::Quat Transform::GetRotation() const
    {
        return m_rotation;
    }

    mathfu::Vector3 Transform::GetScale() const
    {
        return m_scale;
    }

    void Transform::SetPosition(const mathfu::Vector3& position)
    {
        m_position = position;
    }

    void Transform::SetRotation(const mathfu::Quat& rotation)
    {
        m_rotation = rotation;
    }

    void Transform::SetScale(const mathfu::Vector3& scale)
    {
        m_scale = scale;
    }
} // namespace mathfu
