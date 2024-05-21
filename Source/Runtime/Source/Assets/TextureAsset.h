#pragma once

#include <Assets/Asset.h>
#include <Math/Vector2.h>

namespace DX
{
    struct TextureData
    {
        Math::Vector2Int m_size;
        uint8_t* m_data;
    };

    class TextureAsset : public Asset<TextureData>
    {
    public:
        // Loads a texture from a file. The filename is relative to the assets folder.
        static std::shared_ptr<TextureAsset> LoadTextureAsset(const std::string& fileName);

        static inline const AssetType TextureAssetType = 0xB8FCE1BE;

        TextureAsset() = default;
        ~TextureAsset() = default;

        AssetType GetAssetType() const override
        {
            return TextureAssetType;
        }

    protected:
        using Super = Asset<TextureData>;

        TextureAsset(AssetId assetId, std::unique_ptr<TextureData> textureData);
    };
} // namespace DX
