#pragma once

#include <Math/Vector2.h>

#include <string>
#include <filesystem>
#include <optional>

namespace DX
{
    std::optional<std::string> ReadAssetFile(const std::string& fileName);

    uint8_t* LoadTexture(const std::string& fileName, mathfu::Vector2Int& textureSizeOut);

    std::filesystem::path GetAssetPath();

    std::filesystem::path GetExecutablePath();
} // namespace DX
