#pragma once

#include <Renderer/Transform.h>

namespace DX
{
    class Camera
    {
    public:
        Camera() = default;

        mathfu::Matrix4x4 GetViewMatrix() const;
        mathfu::Matrix4x4 GetProjectionMatrix() const;

        void ProcessTransformPosition();

    private:
        Transform m_transform = Transform::CreateIdentity();

        mathfu::Matrix4x4 m_viewMatrix;
        mathfu::Matrix4x4 m_projectionMatrix;
    };
} // namespace DX
