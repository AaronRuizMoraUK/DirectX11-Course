#include <Assets/MeshAsset.h>
#include <Assets/AssetManager.h>
#include <Log/Log.h>

namespace DX
{
    MeshAsset::MeshAsset(AssetId assetId, std::unique_ptr<MeshData> data)
        : Super(assetId, std::move(data))
    {
    }

    std::shared_ptr<MeshAsset> MeshAsset::LoadMeshAsset(const std::string& fileName)
    {
        return DX::AssetManager::Get().LoadAssetAs<MeshAsset>(
            fileName, 
            std::bind(&MeshAsset::LoadMesh, std::placeholders::_1));
    }

    std::unique_ptr<MeshData> MeshAsset::LoadMesh(const std::filesystem::path& fileNamePath)
    {
        auto meshData = std::make_unique<MeshData>();

        return meshData;
    }
} // namespace DX
