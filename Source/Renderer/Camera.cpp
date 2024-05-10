#include <Renderer/Camera.h>
#include <Window/WindowManager.h>

// GLFW uses Vulkan by default, so we need to indicate to not use it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace DX
{
    namespace
    {
        struct CoordinateSystem
        {
            // Right Hand
            //      Y
            //      | 
            //      | 
            //      |
            //      -----> X
            //     /
            //    /
            //   Z 
            static inline const float RightHand = 1.0f;

            // Left Hand
            //      Y
            //      |  Z
            //      | /
            //      |/
            //      -----> X
            static inline const float LeftHand = -1.0f;

            static inline const float Default = RightHand;
        };
    }

    mathfu::Matrix4x4 Camera::GetViewMatrix() const
    {
        return mathfu::Matrix4x4::LookAt(
            m_transform.GetPosition() + m_transform.GetBasisZ(),
            m_transform.GetPosition(), 
            m_transform.GetBasisY(),
            CoordinateSystem::Default);
    }

    mathfu::Matrix4x4 Camera::GetProjectionMatrix() const
    {
        Window* window = WindowManager::Get().GetWindow(0);
        assert(window);

        const float fovY = 74.0f;
        const float aspectRatio = static_cast<float>(window->GetSize().m_width) / static_cast<float>(window->GetSize().m_height);
        const float nearPlane = 0.1f;
        const float farPlane = 1000.0f;

        return mathfu::Matrix4x4::Perspective(
            fovY,
            aspectRatio,
            nearPlane,
            farPlane,
            CoordinateSystem::Default);
    }

    void Camera::ProcessTransformPosition()
    {
        Window* window = WindowManager::Get().GetWindow(0);
        assert(window);

        auto* windowHandler = window->GetWindowHandler();

        constexpr float deltaTime = 1.0f / 60.0f;
        constexpr float speed = 0.1f;

        mathfu::Vector3 deltaMovement(0.0f);

        if (glfwGetKey(windowHandler, GLFW_KEY_W) == GLFW_PRESS)
        {
            deltaMovement += m_transform.GetBasisZ();
        }

        if (glfwGetKey(windowHandler, GLFW_KEY_S) == GLFW_PRESS)
        {
            deltaMovement -= m_transform.GetBasisZ();
        }

        if (glfwGetKey(windowHandler, GLFW_KEY_A) == GLFW_PRESS)
        {
            deltaMovement -= m_transform.GetBasisX();
        }

        if (glfwGetKey(windowHandler, GLFW_KEY_D) == GLFW_PRESS)
        {
            deltaMovement += m_transform.GetBasisX();
        }

        if (glfwGetKey(windowHandler, GLFW_KEY_E) == GLFW_PRESS)
        {
            deltaMovement += m_transform.GetBasisY();
        }

        if (glfwGetKey(windowHandler, GLFW_KEY_Q) == GLFW_PRESS)
        {
            deltaMovement -= m_transform.GetBasisY();
        }

        m_transform.SetPosition(m_transform.GetPosition() + deltaMovement * speed * deltaTime);
    }
} // namespace DX
