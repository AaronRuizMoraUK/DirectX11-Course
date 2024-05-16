#include <Graphics/Shader/ShaderCompiler.h>

#include <File/FileUtils.h>
#include <Log/Log.h>

#include <Graphics/DirectX/ComPtr.h>
#include <Graphics/DirectX/DX11ShaderBytecode.h>
#include <d3dcompiler.h>

namespace DX
{
    static const char* ToDX11CompilerTarget(ShaderType m_shaderType)
    {
        switch (m_shaderType)
        {
        case ShaderType::Vertex: return "vs_5_0";
        case ShaderType::Hull: return "hs_5_0";
        case ShaderType::Domain: return "ds_5_0";
        case ShaderType::Geometry: return "gs_5_0";
        case ShaderType::Compute: return "cs_5_0";
        case ShaderType::Pixel: return "ps_5_0";

        default:
            DX_ASSERT(false, "ShaderCompiler", "Unsupported shader type %d.", m_shaderType);
            return nullptr;
        }
    }

    std::unique_ptr<ShaderBytecode> ShaderCompiler::Compile(const ShaderInfo& shaderInfo)
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

        return std::make_unique<DX11ShaderBytecode>(std::move(shaderBlob));
    }
} // namespace DX
