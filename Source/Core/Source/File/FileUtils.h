#pragma once

#include <Math/Vector2.h>

#include <string>
#include <filesystem>
#include <optional>

namespace DX
{
    // Reads the content of a file.
    // The filename is relative to the assets folder.
    std::optional<std::string> ReadAssetFile(const std::string& fileName);

    // Loads a texture from a file.
    // The filename is relative to the assets folder.
    uint8_t* LoadTexture(const std::string& fileName, mathfu::Vector2Int& textureSizeOut);

    // Returns the path to the assets folder.
    std::filesystem::path GetAssetPath();

    // Returns the path to the executable folder.
    std::filesystem::path GetExecutablePath();
} // namespace DX
