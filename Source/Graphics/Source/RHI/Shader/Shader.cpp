#include <RHI/Shader/Shader.h>

#include <RHI/Shader/ShaderBytecode.h>
#include <RHI/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    static const char* ShaderTypeStr(ShaderType shaderType)
    {
        switch (shaderType)
        {
        case ShaderType_Vertex:
            return "Vertex";
        case ShaderType_Hull:
            return "Hull";
        case ShaderType_Domain:
            return "Domain";
        case ShaderType_Geometry:
            return "Geometry";
        case ShaderType_Pixel:
            return "Pixel";
        case ShaderType_Compute:
            return "Compute";

        case ShaderType_Unknown:
        default:
            return "Unknown";
        }
    }

    Shader::Shader(Device* device, const ShaderDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (!m_desc.m_bytecode)
        {
            DX_LOG(Fatal, "Shader", "Shader description with invalid bytecode.");
            return;
        }

        HRESULT result;

        switch (m_desc.m_shaderInfo.m_shaderType)
        {
        case ShaderType_Vertex:
        {
            ComPtr<ID3D11VertexShader> dx11VertexShader;
            result = m_ownerDevice->GetDX11Device()->CreateVertexShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11VertexShader.GetAddressOf());
            m_dx11Shader = dx11VertexShader;
        }
        break;

        case ShaderType_Hull:
        {
            ComPtr<ID3D11HullShader> dx11HullShader;
            result = m_ownerDevice->GetDX11Device()->CreateHullShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11HullShader.GetAddressOf());
            m_dx11Shader = dx11HullShader;
        }
        break;

        case ShaderType_Domain:
        {
            ComPtr<ID3D11DomainShader> dx11DomainShader;
            result = m_ownerDevice->GetDX11Device()->CreateDomainShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11DomainShader.GetAddressOf());
            m_dx11Shader = dx11DomainShader;
        }
        break;

        case ShaderType_Geometry:
        {
            ComPtr<ID3D11GeometryShader> dx11GeometryShader;
            result = m_ownerDevice->GetDX11Device()->CreateGeometryShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11GeometryShader.GetAddressOf());
            m_dx11Shader = dx11GeometryShader;
        }
        break;

        case ShaderType_Pixel:
        {
            ComPtr<ID3D11PixelShader> dx11PixelShader;
            result = m_ownerDevice->GetDX11Device()->CreatePixelShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11PixelShader.GetAddressOf());
            m_dx11Shader = dx11PixelShader;
        }
        break;

        case ShaderType_Compute:
        {
            ComPtr<ID3D11ComputeShader> dx11ComputeShader;
            result = m_ownerDevice->GetDX11Device()->CreateComputeShader(
                m_desc.m_bytecode->GetData(),
                m_desc.m_bytecode->GetSize(),
                nullptr, // Class linkage
                dx11ComputeShader.GetAddressOf());
            m_dx11Shader = dx11ComputeShader;
        }
        break;

        case ShaderType_Unknown:
        default:
            DX_LOG(Fatal, "Shader", "Shader description with unknown shader type (%d).", m_desc.m_shaderInfo.m_shaderType);
            return;
        };

        if (FAILED(result))
        {
            DX_LOG(Error, "Shader", "Failed to create %s shader", ShaderTypeStr(m_desc.m_shaderInfo.m_shaderType));
            return;
        }

        DX_LOG(Verbose, "Shader", "%s shader '%s' created.", ShaderTypeStr(m_desc.m_shaderInfo.m_shaderType), m_desc.m_shaderInfo.m_name.c_str());
    }

    Shader::~Shader()
    {
        DX_LOG(Verbose, "Shader", "%s shader '%s' destroyed.", ShaderTypeStr(m_desc.m_shaderInfo.m_shaderType), m_desc.m_shaderInfo.m_name.c_str());
    }
} // namespace DX
