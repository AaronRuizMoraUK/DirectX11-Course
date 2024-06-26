#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Shader/ShaderDesc.h>

#include <variant>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11VertexShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11GeometryShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;

namespace DX
{
    // Represents a shader object of any type.
    class Shader : public DeviceObject
    {
    public:
        Shader(Device* device, const ShaderDesc& desc);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Shader; }

        const ShaderDesc& GetShaderDesc() const { return m_desc; }
        ShaderType GetShaderType() const { return m_desc.m_shaderInfo.m_shaderType; }

        // Pass one of the DX11 shader types as the template parameter to get the DX11 shader.
        // If the shader is holding another shader type, the result is nullptr.
        // For example: GetDX11ShaderAs<ID3D11VertexShader>()
        template<typename T>
        T* GetDX11ShaderAs()
        {
            auto* dx11Shader = std::get_if<ComPtr<T>>(&m_dx11Shader);
            return (dx11Shader) ? dx11Shader->Get() : nullptr;
        }

    private:
        ShaderDesc m_desc;

    private:
        using DX11Shader = std::variant<
            ComPtr<ID3D11VertexShader>, 
            ComPtr<ID3D11HullShader>,
            ComPtr<ID3D11DomainShader>,
            ComPtr<ID3D11GeometryShader>,
            ComPtr<ID3D11PixelShader>,
            ComPtr<ID3D11ComputeShader>>;

        DX11Shader m_dx11Shader;
    };
} // namespace DX
