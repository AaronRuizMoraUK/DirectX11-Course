#pragma once

#include <Math/Transform.h>

#include <memory>

namespace DX
{
    class Buffer;

    // 3D perspective camera
    class Camera
    {
    public:
        Camera();
        Camera(const Math::Vector3& position, const Math::Vector3& lookAtPosition);
        explicit Camera(const Math::Transform& transform);
        ~Camera();

        void Update(float deltaTime);

        Math::Transform GetTransform() const { return m_transform; }

        Math::Matrix4x4 GetViewMatrix() const;
        Math::Matrix4x4 GetProjectionMatrix() const;

        void SetBuffers();

    private:
        bool m_firstUpdate = true;
        float m_moveSpeed = 4.0f;
        float m_rotationSensitivity = 3.0f;

        Math::Transform m_transform = Math::Transform::CreateIdentity();

    private:
        void CreateBuffers();

        struct ViewProjBuffer
        {
            Math::Matrix4x4Packed m_viewMatrix;
            Math::Matrix4x4Packed m_projMatrix;
        };

        std::shared_ptr<Buffer> m_viewProjMatrixConstantBuffer;
    };
} // namespace DX
