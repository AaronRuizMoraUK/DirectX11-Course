#include <RHI/Shader/ShaderEnums.h>

namespace DX
{
    const char* ShaderTypeStr(ShaderType shaderType)
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
} // namespace DX
