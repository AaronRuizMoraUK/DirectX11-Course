#include <RHI/Shader/ShaderCompiler/ShaderCompiler.h>

#include <File/FileUtils.h>
#include <Log/Log.h>
#include <Debug/Debug.h>

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
            case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY: return "ShaderResourceView (TextureCubeArray)";
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
            case D3D11_SRV_DIMENSION_TEXTURE3D:        return "ShaderRWResourceView (Texture3D)";
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

    static void AddResourceBindingToLayout(
        const D3D11_SHADER_INPUT_BIND_DESC& dx11ResourceDesc,
        ShaderResourceLayout& shaderResourceLayout)
    {
        switch (dx11ResourceDesc.Type)
        {
        // Constant Buffer
        case D3D_SIT_CBUFFER:
        case D3D_SIT_TBUFFER:
            shaderResourceLayout.m_constantBuffers.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount);
            break;

        // Shader Resource View
        case D3D_SIT_TEXTURE:
            switch (dx11ResourceDesc.Dimension)
            {
            case D3D11_SRV_DIMENSION_TEXTURE1D:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture1D);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture1D, TextureSubType_Array);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2D:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D, TextureSubType_Array);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2DMS:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D, TextureSubType_Multisample);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D, TextureSubType_Array | TextureSubType_Multisample);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE3D:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture3D);
                break;
            case D3D11_SRV_DIMENSION_TEXTURECUBE:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::TextureCube);
                break;
            case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::TextureCube, TextureSubType_Array);
                break;
            case D3D11_SRV_DIMENSION_BUFFER:
                shaderResourceLayout.m_shaderResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Typed);
                break;
            default:
                DX_ASSERT(false, "ShaderCompiler", "Unexpected Shader dimension %d.", dx11ResourceDesc.Dimension);
                break;
            }
            break;

        case D3D_SIT_STRUCTURED:
            shaderResourceLayout.m_shaderResourceViews.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Structured);
            break;

        case D3D_SIT_BYTEADDRESS:
            shaderResourceLayout.m_shaderResourceViews.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Raw);
            break;

        // Shader RW Resource View
        case D3D_SIT_UAV_RWTYPED:
            switch (dx11ResourceDesc.Dimension)
            {
            case D3D11_SRV_DIMENSION_TEXTURE1D:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture1D);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture1D, TextureSubType_Array);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2D:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture2D, TextureSubType_Array);
                break;
            case D3D11_SRV_DIMENSION_TEXTURE3D:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount,
                    TextureType::Texture3D);
                break;
            case D3D11_SRV_DIMENSION_BUFFER:
                shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                    dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Typed);
                break;
            default:
                DX_ASSERT(false, "ShaderCompiler", "Unexpected Shader dimension %d.", dx11ResourceDesc.Dimension);
                break;
            }
            break;

        case D3D_SIT_UAV_RWSTRUCTURED:
            shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Structured);
            break;

        case D3D_SIT_UAV_RWBYTEADDRESS:
            shaderResourceLayout.m_shaderRWResourceViews.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount, BufferType::Raw);
            break;

        // Sampler
        case D3D_SIT_SAMPLER:
            shaderResourceLayout.m_samplers.emplace_back(
                dx11ResourceDesc.Name, dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount);
            break;

        default:
            DX_ASSERT(false, "ShaderCompiler", "Unsupported shader input type %d.", dx11ResourceDesc.Type);
            break;
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
        ShaderResourceLayout shaderResourceLayout;
        {
            ComPtr<ID3D11ShaderReflection> dx11ShaderReflection;
            result = D3DReflect(
                shaderBlob->GetBufferPointer(),
                shaderBlob->GetBufferSize(),
                IID_PPV_ARGS(dx11ShaderReflection.GetAddressOf()));
            if (FAILED(result)) {
                DX_LOG(Error, "ShaderCompiler", "Failed to create shader reflection.");
                return nullptr;
            }

            D3D11_SHADER_DESC dx11ShaderDesc;
            dx11ShaderReflection->GetDesc(&dx11ShaderDesc);

            DX_LOG(Verbose, "ShaderCompiler", "---------------------");
            DX_LOG(Verbose, "ShaderCompiler", "Shader Name: %s", shaderInfo.m_name.c_str());
            for (uint32_t i = 0; i < dx11ShaderDesc.BoundResources; ++i) {
                D3D11_SHADER_INPUT_BIND_DESC dx11ResourceDesc;
                dx11ShaderReflection->GetResourceBindingDesc(i, &dx11ResourceDesc);

                DX_LOG(Verbose, "ShaderCompiler", "- Resource Name: %s Type: %s Bind Point: %u Bind Count: %u",
                    dx11ResourceDesc.Name, ToDX11ResourceString(dx11ResourceDesc.Type, dx11ResourceDesc.Dimension),
                    dx11ResourceDesc.BindPoint, dx11ResourceDesc.BindCount);

                AddResourceBindingToLayout(dx11ResourceDesc, shaderResourceLayout);
            }
            DX_LOG(Verbose, "ShaderCompiler", "---------------------");
        }

        return std::make_shared<DX11ShaderBytecode>(std::move(shaderBlob), std::move(shaderResourceLayout));
    }
} // namespace DX
