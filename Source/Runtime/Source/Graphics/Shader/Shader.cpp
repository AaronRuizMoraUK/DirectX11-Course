#include <Graphics/Shader/Shader.h>

#include <Graphics/Shader/ShaderBytecode.h>
#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    static const char* ShaderTypeStr(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType::Vertex:
            return "Vertex";
        case ShaderType::Hull:
            return "Hull";
        case ShaderType::Domain:
            return "Domain";
        case ShaderType::Geometry:
            return "Geometry";
        case ShaderType::Pixel:
            return "Pixel";
        case ShaderType::Compute:
            return "Compute";

        case ShaderType::Unknown:
        default:
            return "Unknown";
        }
    }

    Shader::Shader(Device* device, const ShaderDesc& desc)
        : DeviceObject(device)
        , m_shaderInfo(desc.m_shaderInfo)
    {
        if (!desc.m_bytecode)
        {
            DX_LOG(Fatal, "Shader", "Shader description with invalid bytecode.");
            return;
        }

        HRESULT result;

        switch (m_shaderInfo.m_shaderType)
        {
        case ShaderType::Vertex:
        {
            ComPtr<ID3D11VertexShader> dx11VertexShader;
            result = m_ownerDevice->GetDX11Device()->CreateVertexShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11VertexShader.GetAddressOf());
            m_dx11Shader = dx11VertexShader;
        }
        break;

        case ShaderType::Hull:
        {
            ComPtr<ID3D11HullShader> dx11HullShader;
            result = m_ownerDevice->GetDX11Device()->CreateHullShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11HullShader.GetAddressOf());
            m_dx11Shader = dx11HullShader;
        }
        break;

        case ShaderType::Domain:
        {
            ComPtr<ID3D11DomainShader> dx11DomainShader;
            result = m_ownerDevice->GetDX11Device()->CreateDomainShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11DomainShader.GetAddressOf());
            m_dx11Shader = dx11DomainShader;
        }
        break;

        case ShaderType::Geometry:
        {
            ComPtr<ID3D11GeometryShader> dx11GeometryShader;
            result = m_ownerDevice->GetDX11Device()->CreateGeometryShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11GeometryShader.GetAddressOf());
            m_dx11Shader = dx11GeometryShader;
        }
        break;

        case ShaderType::Pixel:
        {
            ComPtr<ID3D11PixelShader> dx11PixelShader;
            result = m_ownerDevice->GetDX11Device()->CreatePixelShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11PixelShader.GetAddressOf());
            m_dx11Shader = dx11PixelShader;
        }
        break;

        case ShaderType::Compute:
        {
            ComPtr<ID3D11ComputeShader> dx11ComputeShader;
            result = m_ownerDevice->GetDX11Device()->CreateComputeShader(
                desc.m_bytecode->GetData(),
                desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11ComputeShader.GetAddressOf());
            m_dx11Shader = dx11ComputeShader;
        }
        break;

        case ShaderType::Unknown:
        default:
            DX_LOG(Fatal, "Shader", "Shader description with unknown shader type (%d).", m_shaderInfo.m_shaderType);
            return;
        };

        if (FAILED(result))
        {
            DX_LOG(Error, "Shader", "Failed to create %s shader", ShaderTypeStr(m_shaderInfo.m_shaderType));
            return;
        }

        DX_LOG(Verbose, "Buffer", "Shader created.");
    }

    Shader::~Shader()
    {
        DX_LOG(Verbose, "Buffer", "Shader destroyed.");
    }
} // namespace DX
