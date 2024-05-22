#pragma once

#include <string>

namespace DX
{
    enum ShaderType
    {
        ShaderType_Unknown = 0,

        ShaderType_Vertex,
        ShaderType_Hull,
        ShaderType_Domain,
        ShaderType_Geometry,
        ShaderType_Pixel,
        ShaderType_Compute,

        ShaderType_Count
    };

    const char* ShaderTypeStr(ShaderType shaderType);

    struct ShaderInfo
    {
        ShaderType m_shaderType;
        std::string m_name;
        std::string m_entryPoint;
    };
} // namespace DX
