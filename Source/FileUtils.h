#pragma once

#include <string>
#include <filesystem>
#include <optional>
#include <Math/Vector2.h>

namespace DX
{
    std::optional<std::string> ReadAssetFile(const std::string& fileName);

    uint8_t* LoadTexture(const std::string& fileName, mathfu::Vector2Int& textureSizeOut);

    std::filesystem::path GetAssetPath();

    std::filesystem::path GetExecutablePath();
} // namespace DX
