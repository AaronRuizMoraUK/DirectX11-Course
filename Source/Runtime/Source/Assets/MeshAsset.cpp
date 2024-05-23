#include <Assets/MeshAsset.h>
#include <Assets/AssetManager.h>
#include <Log/Log.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
        Assimp::Importer importer;

        const uint32_t importerFlags = 
            aiProcess_Triangulate |
            aiProcess_ConvertToLeftHanded |
            aiProcess_GenSmoothNormals |
            aiProcess_CalcTangentSpace |
            aiProcess_JoinIdenticalVertices;

        const aiScene* scene = importer.ReadFile(fileNamePath.generic_string(), importerFlags);

        if (!scene || 
            !scene->mRootNode ||
            scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
        {
            DX_LOG(Error, "MeshAsset", "Assimp failed to import mesh: %s\n\nError message: %s\n", 
                fileNamePath.generic_string().c_str(), importer.GetErrorString());
            return nullptr;
        }

        if (!scene->HasMeshes())
        {
            DX_LOG(Error, "MeshAsset", "Assimp failed to import mesh: %s\n\nError message: %s\n",
                fileNamePath.generic_string().c_str(), importer.GetErrorString());
            return nullptr;
        }

        const aiMesh* mesh = scene->mMeshes[0]; // TODO: Support multiple meshes

        auto meshData = std::make_unique<MeshData>();

        if (mesh->HasPositions())
        {
            meshData->m_positions.resize(mesh->mNumVertices);
            std::transform(mesh->mVertices, mesh->mVertices + mesh->mNumVertices,
                meshData->m_positions.begin(),
                [](const aiVector3D& lhs)
                {
                    Math::Vector3Packed rhs;
                    rhs.x = lhs.x;
                    rhs.y = lhs.y;
                    rhs.z = lhs.z;
                    return rhs;
                });

            meshData->m_indices.resize(mesh->mNumFaces * 3);
            for (int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
            {
                DX_ASSERT(mesh->mFaces[faceIndex].mNumIndices == 3, "MeshAsset", "Mesh face must have 3 indices");

                const int index = faceIndex * 3;
                meshData->m_indices[index + 0] = mesh->mFaces[faceIndex].mIndices[0];
                meshData->m_indices[index + 1] = mesh->mFaces[faceIndex].mIndices[1];
                meshData->m_indices[index + 2] = mesh->mFaces[faceIndex].mIndices[2];
            }
        }
        else
        {
            DX_LOG(Error, "MeshAsset", "Mesh %s has no positions\n", fileNamePath.generic_string().c_str());
            return nullptr;
        }

        // Use first set of texture coordinates
        if (mesh->HasTextureCoords(0))
        {
            meshData->m_textCoords.resize(mesh->mNumVertices);
            std::transform(mesh->mTextureCoords[0], mesh->mTextureCoords[0] + mesh->mNumVertices,
                meshData->m_textCoords.begin(),
                [](const aiVector3D& lhs)
                {
                    Math::Vector2Packed rhs;
                    rhs.x = lhs.x;
                    rhs.y = lhs.y;
                    return rhs;
                });
        }
        else
        {
            DX_LOG(Warning, "MeshAsset", "Mesh %s has no texture coordinates\n", fileNamePath.generic_string().c_str());
            return nullptr;
        }

        if (mesh->HasNormals())
        {
            meshData->m_normals.resize(mesh->mNumVertices);
            std::transform(mesh->mNormals, mesh->mNormals + mesh->mNumVertices,
                meshData->m_normals.begin(),
                [](const aiVector3D& lhs)
                {
                    Math::Vector3Packed rhs;
                    rhs.x = lhs.x;
                    rhs.y = lhs.y;
                    rhs.z = lhs.z;
                    return rhs;
                });
        }
        else
        {
            DX_LOG(Warning, "MeshAsset", "Mesh %s has no normals\n", fileNamePath.generic_string().c_str());
            return nullptr;
        }

        if (mesh->HasTangentsAndBitangents())
        {
            meshData->m_tangents.resize(mesh->mNumVertices);
            std::transform(mesh->mTangents, mesh->mTangents + mesh->mNumVertices,
                meshData->m_tangents.begin(),
                [](const aiVector3D& lhs)
                {
                    Math::Vector3Packed rhs;
                    rhs.x = lhs.x;
                    rhs.y = lhs.y;
                    rhs.z = lhs.z;
                    return rhs;
                });

            meshData->m_binormals.resize(mesh->mNumVertices);
            std::transform(mesh->mBitangents, mesh->mBitangents + mesh->mNumVertices,
                meshData->m_binormals.begin(),
                [](const aiVector3D& lhs)
                {
                    Math::Vector3Packed rhs;
                    rhs.x = lhs.x;
                    rhs.y = lhs.y;
                    rhs.z = lhs.z;
                    return rhs;
                });
        }
        else
        {
            DX_LOG(Warning, "MeshAsset", "Mesh %s has no tangents and binormals\n", fileNamePath.generic_string().c_str());
            return nullptr;
        }

        return meshData;
    }
} // namespace DX
