#include <Renderer/Camera.h>
#include <Renderer/RendererManager.h>
#include <Window/WindowManager.h>
#include <Log/Log.h>

#include <Math/Vector2.h>
#include <Math/Matrix3x3.h>
#include <mathfu/constants.h>

#include <d3d11.h>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace DX
{
    Camera::Camera()
    {
        CreateBuffers();
    }

    Camera::Camera(const mathfu::Vector3& position, const mathfu::Vector3& lookAtPosition)
    {
        m_transform.m_position = position;
        if (const auto basisZ = (lookAtPosition - position);
            basisZ.Length() > 1e-5f)
        {
            m_transform.m_rotation = mathfu::CreateQuatFromBasisZ(basisZ.Normalized());
        }

        CreateBuffers();
    }

    Camera::Camera(const mathfu::Transform& transform)
        : m_transform(transform)
    {
        CreateBuffers();
    }

    void Camera::CreateBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        DX_ASSERT(renderer, "Camera", "Renderer 0 not found");

        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            constantBufferDesc.ByteWidth = sizeof(ViewProjBuffer);
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
            constantBufferDesc.MiscFlags = 0;

            const ViewProjBuffer viewProjBuffer = { GetViewMatrix() , GetProjectionMatrix() };

            D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
            constantSubresourceData.pSysMem = &viewProjBuffer;
            constantSubresourceData.SysMemPitch = 0;
            constantSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, m_viewProjMatrixConstantBuffer.GetAddressOf());
        }
    }

    Camera::~Camera() = default;

    void Camera::Update(float deltaTime)
    {
        Window* window = WindowManager::Get().GetWindow(0);
        DX_ASSERT(window, "Camera", "Window 0 not found");

        auto* windowHandler = window->GetWindowHandler();

        if (glfwGetMouseButton(windowHandler, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            // Hide mouse cursor
            glfwSetInputMode(windowHandler, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else if (glfwGetMouseButton(windowHandler, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
        {
            // Show mouse cursor
            glfwSetInputMode(windowHandler, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            m_firstUpdate = true;
            return;
        }

        // Speed
        {
            m_moveSpeed = std::clamp(m_moveSpeed + 0.2f * window->GetScrollOffset(), 0.1f, 1000.0f);
        }

        // Movement
        {
            mathfu::Vector3 deltaMovement(0.0f);

            // Forward/Backward
            if (glfwGetKey(windowHandler, GLFW_KEY_W) == GLFW_PRESS)
            {
                deltaMovement += m_transform.GetBasisZ();
            }
            if (glfwGetKey(windowHandler, GLFW_KEY_S) == GLFW_PRESS)
            {
                deltaMovement -= m_transform.GetBasisZ();
            }
            // Left/Right (LeftHand)
            if (glfwGetKey(windowHandler, GLFW_KEY_A) == GLFW_PRESS)
            {
                deltaMovement -= m_transform.GetBasisX();
            }
            if (glfwGetKey(windowHandler, GLFW_KEY_D) == GLFW_PRESS)
            {
                deltaMovement += m_transform.GetBasisX();
            }

            // Up/Down
            if (glfwGetKey(windowHandler, GLFW_KEY_E) == GLFW_PRESS)
            {
                deltaMovement += mathfu::kAxisY3f;
            }
            if (glfwGetKey(windowHandler, GLFW_KEY_Q) == GLFW_PRESS)
            {
                deltaMovement -= mathfu::kAxisY3f;
            }

            m_transform.m_position += deltaMovement * m_moveSpeed * deltaTime;
        }

        // Rotation
        {
            const mathfu::Vector2 windowSize(
                static_cast<float>(window->GetSize().x),
                static_cast<float>(window->GetSize().y));

            if (m_firstUpdate)
            {
                glfwSetCursorPos(windowHandler, 0.5f * windowSize.x, 0.5f * windowSize.y);
                m_firstUpdate = false;
            }

            double mouseX = 0.0f;
            double mouseY = 0.0f;
            glfwGetCursorPos(windowHandler, &mouseX, &mouseY);

            const mathfu::Vector2 mousePosition(
                static_cast<float>(mouseX),
                static_cast<float>(mouseY));

            mathfu::Vector2 delta = m_rotationSensitivity * (mousePosition - 0.5f * windowSize) / windowSize;

            // Clamp pitch to avoid camera looking straight up or down.
            // Negative pitch delta makes camera look higher.
            const float angle = acosf(mathfu::Vector3::DotProduct(m_transform.GetBasisZ(), mathfu::kAxisY3f)) * mathfu::kRadiansToDegrees;
            const float minAngle = 10.0f;
            if ((angle <= minAngle && delta.y < 0.0f) ||          // Avoid looking higher than 10 degrees to up axis
                (angle >= (180.0f - minAngle) && delta.y > 0.0f)) // Avoid looking lower  than 10 degrees to -up axis
            {
                delta.y = 0.0f;
            }

            m_transform.m_rotation =
                mathfu::Quat::FromAngleAxis(delta.x, mathfu::kAxisY3f) * // Apply yaw in world space to orbit around up axis (LeftHand)
                m_transform.m_rotation * 
                mathfu::Quat::FromAngleAxis(delta.y, mathfu::kAxisX3f); // Apply pitch in local space

            // Reset cursor position to the center of the window
            glfwSetCursorPos(windowHandler, 0.5f * windowSize.x, 0.5f * windowSize.y);
        }
    }

    mathfu::Matrix4x4 Camera::GetViewMatrix() const
    {
        return mathfu::Matrix4x4::LookAt(
            m_transform.m_position + m_transform.GetBasisZ(),
            m_transform.m_position,
            m_transform.GetBasisY(),
            mathfu::CoordinateSystem::Default);
    }

    mathfu::Matrix4x4 Camera::GetProjectionMatrix() const
    {
        Window* window = WindowManager::Get().GetWindow(0);
        DX_ASSERT(window, "Camera", "Window 0 not found");

        const float fovY = 74.0f * mathfu::kDegreesToRadians;
        const float aspectRatio = static_cast<float>(window->GetSize().x) / static_cast<float>(window->GetSize().y);
        const float nearPlane = 0.1f;
        const float farPlane = 1000.0f;

        return mathfu::Matrix4x4::Perspective(
            fovY,
            aspectRatio,
            nearPlane,
            farPlane,
            mathfu::CoordinateSystem::Default);
    }

    void Camera::SetBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        DX_ASSERT(renderer, "Camera", "Renderer 0 not found");

        // Update constant buffer with the latest view and projection matrices.
        {
            const ViewProjBuffer viewProjBuffer = { GetViewMatrix() , GetProjectionMatrix() };

            D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
            renderer->GetDeviceContext()->Map(m_viewProjMatrixConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
            memcpy(mappedSubresource.pData, &viewProjBuffer, sizeof(ViewProjBuffer));
            renderer->GetDeviceContext()->Unmap(m_viewProjMatrixConstantBuffer.Get(), 0);
        }

        renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_viewProjMatrixConstantBuffer.GetAddressOf());
    }
} // namespace DX
