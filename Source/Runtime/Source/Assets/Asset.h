#pragma once

#include <string>
#include <memory>

namespace DX
{
    using AssetId = std::string;

    using AssetType = uint32_t;

    class AssetBase
    {
    public:
        AssetBase(AssetId assetId)
            : m_assetId(assetId)
        {
        }

        virtual ~AssetBase() = default;

        AssetBase(const AssetBase&) = delete;
        AssetBase& operator=(const AssetBase&) = delete;

        const AssetId& GetAssetId() const
        {
            return m_assetId;
        }

        virtual AssetType GetAssetType() const = 0;

    protected:
        AssetId m_assetId;
    };

    template<typename T>
    class Asset
    {
    public:
        Asset(AssetId assetId)
            : AssetBase(assetId)
        {
        }

        Asset(AssetId assetId, std::unique_ptr<T> data)
            : AssetBase(assetId)
            , m_data(std::move(data))
        {
        }

        const T* GetData() const
        {
            return m_data.get();
        }

    protected:
        std::unique_ptr<T> m_data;
    };
} // namespace DX
