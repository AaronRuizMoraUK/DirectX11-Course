#include <Assets/TextureAsset.h>
#include <Assets/AssetManager.h>
#include <File/FileUtils.h>
#include <Log/Log.h>

#include <stb_image.h>

namespace DX
{
    TextureAsset::TextureAsset(AssetId assetId, std::unique_ptr<TextureData> textureData)
        : Super(assetId, std::move(textureData))
    {
    }

    std::shared_ptr<TextureAsset> TextureAsset::LoadTextureAsset(const std::string& fileName)
    {
        if (auto asset = DX::AssetManager::Get().GetAsset(fileName))
        {
            if (asset->GetAssetType() == TextureAssetType)
            {
                return std::static_pointer_cast<TextureAsset>(asset);
            }
            else
            {
                DX_LOG(Error, "TextureAsset", "An asset that is not a Texture Asset already exists with Id %s.", fileName.c_str());
                return {};
            }
        }

        auto fileNamePath = GetAssetPath() / fileName;
        if (!std::filesystem::exists(fileNamePath))
        {
            DX_LOG(Error, "TextureAsset", "Filename path %s does not exist.", fileNamePath.generic_string().c_str());
            return nullptr;
        }

        auto textureData = std::make_unique<TextureData>();

        textureData->m_data = stbi_load(
            fileNamePath.generic_string().c_str(), 
            &textureData->m_size.x, 
            &textureData->m_size.y, 
            nullptr, 
            STBI_rgb_alpha);

        if (!textureData->m_data)
        {
            DX_LOG(Error, "TextureAsset", "Failed to load texture %s.", fileNamePath.generic_string().c_str());
            return {};
        }

        std::shared_ptr<TextureAsset> textureAsset(new TextureAsset(fileName, std::move(textureData)));

        DX::AssetManager::Get().AddAsset(textureAsset);

        return textureAsset;
    }
} // namespace DX
