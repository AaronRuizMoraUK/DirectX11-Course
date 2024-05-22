#include <RHI/Resource/Texture/Texture.h>

namespace DX
{
    const char* TextureTypeStr(TextureType textureType)
    {
        switch (textureType)
        {
        case TextureType::Texture1D:
            return "1D";
        case TextureType::Texture2D:
            return "2D";
        case TextureType::TextureCube:
            return "Cube";
        case TextureType::Texture3D:
            return "3D";
        default:
            return "Unknown";
        }
    }
} // namespace DX
