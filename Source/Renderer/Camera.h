#pragma once

#include <Math/Transform.h>

// For COM objects' smart pointers
#include <wrl.h>
using Microsoft::WRL::ComPtr;

class ID3D11Buffer;

namespace DX
{
    class Camera
    {
    public:
        Camera();
        Camera(const mathfu::Transform& transform);
        ~Camera();

        void Update(float deltaTime);

        mathfu::Transform GetTransform() const { return m_transform; }

        mathfu::Matrix4x4 GetViewMatrix() const;
        mathfu::Matrix4x4 GetProjectionMatrix() const;

        void SetBuffers();

    private:
        void CreateBuffers();

        mathfu::Transform m_transform = mathfu::Transform::CreateIdentity();

        ComPtr<ID3D11Buffer> m_viewProjMatrixConstantBuffer;

        struct ViewProjBuffer
        {
            mathfu::Matrix4x4Packed m_viewMatrix = mathfu::Matrix4x4::Identity();
            mathfu::Matrix4x4Packed m_projMatrix = mathfu::Matrix4x4::Identity();
        };
        ViewProjBuffer m_viewProjBuffer;
    };
} // namespace DX