#pragma once

#include <Assets/Asset.h>

#include <memory>
#include <unordered_map>

namespace DX
{
    class AssetManager
    {
    public:
        static AssetManager& Get();
        static void Destroy();

        ~AssetManager();

        // Delete copy constructor and assignment operator to prevent copying
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        void AddAsset(std::shared_ptr<AssetBase> asset);
        void RemoveAsset(AssetId assetId);

        std::shared_ptr<AssetBase> GetAsset(AssetId assetId);

        template<typename T>
        std::shared_ptr<T> GetAssetAs(AssetId assetId);

    private:
        AssetManager();

        static std::unique_ptr<AssetManager> Instance;

        using Assets = std::unordered_map<AssetId, std::shared_ptr<AssetBase>>;

        Assets m_assets;
    };

    template<typename T>
    std::shared_ptr<T> AssetManager::GetAssetAs(AssetId assetId)
    {
        if (auto it = m_assets.find(assetId);
            it != m_assets.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }
        return {};
    }

} // namespace DX
