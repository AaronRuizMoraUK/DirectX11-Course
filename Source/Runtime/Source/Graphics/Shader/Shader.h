#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Shader/ShaderDesc.h>

#include <variant>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11VertexShader;
class ID3D11HullShader;
class ID3D11DomainShader;
class ID3D11GeometryShader;
class ID3D11PixelShader;
class ID3D11ComputeShader;

namespace DX
{
    class Shader : public DeviceObject
    {
    public:
        Shader(Device* device, const ShaderDesc& desc);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Shader; }

        const ShaderDesc& GetShaderDesc() const { return m_desc; }

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
