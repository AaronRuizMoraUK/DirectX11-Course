#include <Renderer/Camera.h>
#include <Renderer/RendererManager.h>
#include <Window/WindowManager.h>

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

    Camera::Camera(const mathfu::Transform& transform)
        : m_transform(transform)
    {
        CreateBuffers();
    }

    void Camera::CreateBuffers()
    {
        auto* renderer = RendererManager::Get().GetRenderer(0);
        assert(renderer);

        {
            D3D11_BUFFER_DESC constantBufferDesc = {};
            constantBufferDesc.ByteWidth = sizeof(ViewProjBuffer);
            constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
            constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            constantBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
            constantBufferDesc.MiscFlags = 0;

            D3D11_SUBRESOURCE_DATA constantSubresourceData = {};
            constantSubresourceData.pSysMem = &m_viewProjBuffer;
            constantSubresourceData.SysMemPitch = 0;
            constantSubresourceData.SysMemSlicePitch = 0;

            renderer->GetDevice()->CreateBuffer(&constantBufferDesc, &constantSubresourceData, m_viewProjMatrixConstantBuffer.GetAddressOf());
        }
    }

    Camera::~Camera() = default;

    void Camera::Update(float deltaTime)
    {
        Window* window = WindowManager::Get().GetWindow(0);
        assert(window);

        auto* windowHandler = window->GetWindowHandler();

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

        constexpr float speed = 10.0f;

        m_transform.SetPosition(m_transform.GetPosition() + deltaMovement * speed * deltaTime);
    }

    mathfu::Matrix4x4 Camera::GetViewMatrix() const
    {
        return mathfu::Matrix4x4::LookAt(
            m_transform.GetPosition() + m_transform.GetBasisZ(),
            m_transform.GetPosition(),
            m_transform.GetBasisY(),
            mathfu::CoordinateSystem::Default);
    }

    mathfu::Matrix4x4 Camera::GetProjectionMatrix() const
    {
        Window* window = WindowManager::Get().GetWindow(0);
        assert(window);

        const float fovY = 74.0f * mathfu::kDegreesToRadians;
        const float aspectRatio = static_cast<float>(window->GetSize().m_width) / static_cast<float>(window->GetSize().m_height);
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
        assert(renderer);

        m_viewProjBuffer.m_viewMatrix = GetViewMatrix();
        m_viewProjBuffer.m_projMatrix = GetProjectionMatrix();

        // Update constant buffer with the latest view and projection matrices.
        {
            D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
            renderer->GetDeviceContext()->Map(m_viewProjMatrixConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
            memcpy(mappedSubresource.pData, &m_viewProjBuffer, sizeof(ViewProjBuffer));
            renderer->GetDeviceContext()->Unmap(m_viewProjMatrixConstantBuffer.Get(), 0);
        }

        renderer->GetDeviceContext()->VSSetConstantBuffers(0, 1, m_viewProjMatrixConstantBuffer.GetAddressOf());
    }
} // namespace DX
