#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>

#include <File/FileUtils.h>
#include <Log/Log.h>

#include <RHI/DirectX/ComPtr.h>
#include <RHI/DirectX/DX11ShaderBytecode.h>
#include <d3dcompiler.h>

#include <d3d11.h>

namespace DX
{
    static const char* ToDX11CompilerTarget(ShaderType m_shaderType)
    {
        switch (m_shaderType)
        {
        case ShaderType_Vertex: return "vs_5_0";
        case ShaderType_Hull: return "hs_5_0";
        case ShaderType_Domain: return "ds_5_0";
        case ShaderType_Geometry: return "gs_5_0";
        case ShaderType_Compute: return "cs_5_0";
        case ShaderType_Pixel: return "ps_5_0";

        default:
            DX_ASSERT(false, "ShaderCompiler", "Unsupported shader type %d.", m_shaderType);
            return nullptr;
        }
    }

    static const char* ToDX11ResourceString(D3D_SHADER_INPUT_TYPE type, D3D_SRV_DIMENSION dimension)
    {
        switch (type)
        {
        case D3D_SIT_CBUFFER:
        case D3D_SIT_TBUFFER:
            return "ConstantBuffer";

        case D3D_SIT_TEXTURE:
            switch (dimension)
            {
            case D3D11_SRV_DIMENSION_TEXTURE1D:        return "ShaderResourceView (Texture1D)";
            case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:   return "ShaderResourceView (Texture1DArray)";
            case D3D11_SRV_DIMENSION_TEXTURE2D:        return "ShaderResourceView (Texture2D)";
            case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:   return "ShaderResourceView (Texture2DArray)";
            case D3D11_SRV_DIMENSION_TEXTURE2DMS:      return "ShaderResourceView (Texture2DMS)";
            case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY: return "ShaderResourceView (Texture2DMSArray)";
            case D3D11_SRV_DIMENSION_TEXTURE3D:        return "ShaderResourceView (Texture3D)";
            case D3D11_SRV_DIMENSION_TEXTURECUBE:      return "ShaderResourceView (TextureCube)";
            case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY: return "ShaderResourceView (TextureArray)";
            case D3D11_SRV_DIMENSION_BUFFER:           return "ShaderResourceView (TypedBuffer)";
            default:
                DX_ASSERT(false, "ShaderCompiler", "Unexpected Shader dimension %d.", dimension);
                return nullptr;
            }
        case D3D_SIT_STRUCTURED:  return "ShaderResourceView (StructuredBuffer)";
        case D3D_SIT_BYTEADDRESS: return "ShaderResourceView (RawBuffer)";

        case D3D_SIT_UAV_RWTYPED:
            switch (dimension)
            {
            case D3D11_SRV_DIMENSION_TEXTURE1D:        return "ShaderRWResourceView (Texture1D)";
            case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:   return "ShaderRWResourceView (Texture1DArray)";
            case D3D11_SRV_DIMENSION_TEXTURE2D:        return "ShaderRWResourceView (Texture2D)";
            case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:   return "ShaderRWResourceView (Texture2DArray)";
            case D3D11_SRV_DIMENSION_TEXTURE2DMS:      return "ShaderRWResourceView (Texture2D with MS)";
            case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY: return "ShaderRWResourceView (Texture2DArray with MS)";
            case D3D11_SRV_DIMENSION_TEXTURE3D:        return "ShaderRWResourceView (Texture3D)";
            case D3D11_SRV_DIMENSION_TEXTURECUBE:      return "ShaderRWResourceView (TextureCube)";
            case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY: return "ShaderRWResourceView (TextureArray)";
            case D3D11_SRV_DIMENSION_BUFFER:           return "ShaderRWResourceView (TypedBuffer)";
            default:
                DX_ASSERT(false, "ShaderCompiler", "Unexpected Shader dimension %d.", dimension);
                return nullptr;
            }
        case D3D_SIT_UAV_RWSTRUCTURED:  return "ShaderRWResourceView (StructuredBuffer)";
        case D3D_SIT_UAV_RWBYTEADDRESS: return "ShaderRWResourceView (RawBuffer)";

        case D3D_SIT_SAMPLER:
            return "Sampler";

        default:
            DX_ASSERT(false, "ShaderCompiler", "Unsupported shader input type %d.", type);
            return nullptr;
        }
    }

    std::shared_ptr<ShaderBytecode> ShaderCompiler::Compile(const ShaderInfo& shaderInfo)
    {
        const auto shaderCode = ReadAssetFile(shaderInfo.m_name);
        if (!shaderCode.has_value())
        {
            return nullptr;
        }

        ComPtr<ID3DBlob> shaderBlob;
        ComPtr<ID3DBlob> errorBlob;

        auto result = D3DCompile(
            shaderCode->c_str(),
            shaderCode->length(),
            nullptr, // Source name
            nullptr, // Macros for shader
            nullptr, // Includes for shader
            shaderInfo.m_entryPoint.c_str(),
            ToDX11CompilerTarget(shaderInfo.m_shaderType),
            D3DCOMPILE_ENABLE_STRICTNESS, // Flags from D3D compile constants
            0, // Flags from D3D compile effect constants
            shaderBlob.GetAddressOf(),
            errorBlob.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Error, "ShaderCompiler", "Failed to compile shader %s.", shaderInfo.m_name.c_str());
            if (errorBlob && errorBlob->GetBufferPointer())
            {
                DX_LOG(Error, "ShaderCompiler", "Message from shader compiler:\n%s\n", static_cast<char*>(errorBlob->GetBufferPointer()));
            }
            return nullptr;
        }

        DX_LOG(Verbose, "ShaderCompiler", "Shader '%s' (entry point: '%s') compiled successfully.",
            shaderInfo.m_name.c_str(), shaderInfo.m_entryPoint.c_str());

        // Shader resource layout obtained from shader reflection data
        {
            ComPtr<ID3D11ShaderReflection> shaderReflection;
            result = D3DReflect(
                shaderBlob->GetBufferPointer(),
                shaderBlob->GetBufferSize(),
                IID_PPV_ARGS(shaderReflection.GetAddressOf()));
            if (FAILED(result)) {
                DX_LOG(Error, "ShaderCompiler", "Failed to create shader reflection.");
                return nullptr;
            }

            D3D11_SHADER_DESC shaderDesc;
            shaderReflection->GetDesc(&shaderDesc);

            DX_LOG(Info, "ShaderCompiler", "---------------------");
            DX_LOG(Info, "ShaderCompiler", "Shader Name: %s", shaderInfo.m_name.c_str());
            for (int i = 0; i < shaderDesc.BoundResources; ++i) {
                D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
                shaderReflection->GetResourceBindingDesc(i, &resourceDesc);

                DX_LOG(Info, "ShaderCompiler", "- Resource Name: %s Type: %s Bind Point: %u Bind Count: %u", 
                    resourceDesc.Name, ToDX11ResourceString(resourceDesc.Type, resourceDesc.Dimension), resourceDesc.BindPoint, resourceDesc.BindCount);
            }
            DX_LOG(Info, "ShaderCompiler", "---------------------");
        }

        return std::make_shared<DX11ShaderBytecode>(std::move(shaderBlob));
    }
} // namespace DX
