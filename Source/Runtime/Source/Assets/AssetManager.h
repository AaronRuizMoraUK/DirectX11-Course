#pragma once

#include <Singleton/Singleton.h>
#include <Assets/Asset.h>

#include <memory>
#include <unordered_map>

namespace DX
{
    // TODO: Improve so types can be registered with the AssetManager and
    //       the manager can create the Asset.
    // TODO: Handle loading the asset asynchronously
    // TODO: Handle reloading assets

    class AssetManager : public Singleton<AssetManager>
    {
        friend class Singleton<AssetManager>;
        AssetManager();

    public:
        ~AssetManager();

        void AddAsset(std::shared_ptr<AssetBase> asset);
        void RemoveAsset(AssetId assetId);

        std::shared_ptr<AssetBase> GetAsset(AssetId assetId);

        template<typename T>
        std::shared_ptr<T> GetAssetAs(AssetId assetId);

    private:
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
