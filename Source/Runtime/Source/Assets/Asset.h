#pragma once

#include <string>
#include <memory>
#include <cstdint>

namespace DX
{
    using AssetId = std::string;

    using AssetType = uint32_t;

    class AssetBase
    {
    public:
        virtual ~AssetBase() = default;

        AssetBase(const AssetBase&) = delete;
        AssetBase& operator=(const AssetBase&) = delete;

        const AssetId& GetAssetId() const
        {
            return m_assetId;
        }

        bool IsAssetIdValid() const
        {
            return m_assetId != "";
        }

        virtual AssetType GetAssetType() const = 0;

    protected:
        AssetBase(AssetId assetId)
            : m_assetId(assetId)
        {
        }

    private:
        AssetId m_assetId;
    };

    template<typename T>
    class Asset : public AssetBase
    {
    public:
        const T* GetData() const
        {
            return m_data.get();
        }

    protected:
        Asset(AssetId assetId, std::unique_ptr<T> data)
            : AssetBase(assetId)
            , m_data(std::move(data))
        {
        }

        std::unique_ptr<T> m_data;
    };
} // namespace DX
