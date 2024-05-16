#include <Graphics/Resource/Texture/Texture.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    Texture::Texture(Device* device, const TextureDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (desc.m_dataIsNativeResource)
        {
            if (!desc.m_data)
            {
                DX_LOG(Fatal, "Texture", "Texture description with invalid data.");
                return;
            }

            m_dx11Texture = static_cast<ID3D11Resource*>(desc.m_data);
        }
        else if (desc.m_variant == TextureVariant::Texture1D)
        {
            D3D11_TEXTURE1D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.ArraySize = desc.m_arraySize;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_data)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipLevels, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arraySize, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_data);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for(int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_size.x >> mipIndex);
                        const int rowBytes = ResourceFormatSize(desc.m_format, mipSizeX);

                        subresourceData[index].pSysMem = head;
                        subresourceData[index].SysMemPitch = 0;
                        subresourceData[index].SysMemSlicePitch = 0;

                        head += rowBytes;
                    }
                }
            }

            ComPtr<ID3D11Texture1D> dx11Texture;
            auto result = m_ownerDevice->GetDX11Device()->CreateTexture1D(
                &textureDesc,
                (subresourceData.empty()) ? nullptr : subresourceData.data(),
                dx11Texture.GetAddressOf());

            if (FAILED(result))
            {
                DX_LOG(Fatal, "Texture", "Failed to create 1D texture.");
                return;
            }

            m_dx11Texture = dx11Texture;
        }
        else if (desc.m_variant == TextureVariant::Texture2D)
        {
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.Height = desc.m_size.y;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.ArraySize = desc.m_arraySize;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.SampleDesc.Count = desc.m_sampleCount;
            textureDesc.SampleDesc.Quality = desc.m_sampleQuality;
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_data)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipLevels, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arraySize, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_data);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for (int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_size.x >> mipIndex);
                        const int mipSizeY = std::max<uint32_t>(1, desc.m_size.y >> mipIndex);
                        const int rowBytes = ResourceFormatSize(desc.m_format, mipSizeX);

                        subresourceData[index].pSysMem = head;
                        subresourceData[index].SysMemPitch = rowBytes;
                        subresourceData[index].SysMemSlicePitch = 0;

                        head += rowBytes * mipSizeY;
                    }
                }
            }

            ComPtr<ID3D11Texture2D> dx11Texture;
            auto result = m_ownerDevice->GetDX11Device()->CreateTexture2D(
                &textureDesc,
                (subresourceData.empty()) ? nullptr : subresourceData.data(),
                dx11Texture.GetAddressOf());

            if (FAILED(result))
            {
                DX_LOG(Fatal, "Texture", "Failed to create 2D texture.");
                return;
            }

            m_dx11Texture = dx11Texture;
        }
        else if (desc.m_variant == TextureVariant::Texture3D)
        {
            D3D11_TEXTURE3D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.Height = desc.m_size.y;
            textureDesc.Depth = desc.m_size.z;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_data)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipLevels, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arraySize, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_data);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for (int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_size.x >> mipIndex);
                        const int mipSizeY = std::max<uint32_t>(1, desc.m_size.y >> mipIndex);
                        const int mipSizeZ = std::max<uint32_t>(1, desc.m_size.z >> mipIndex);
                        const int rowBytes = ResourceFormatSize(desc.m_format, mipSizeX);

                        subresourceData[index].pSysMem = head;
                        subresourceData[index].SysMemPitch = rowBytes;
                        subresourceData[index].SysMemSlicePitch = rowBytes * mipSizeY;

                        head += rowBytes * mipSizeY * mipSizeZ;
                    }
                }
            }

            ComPtr<ID3D11Texture3D> dx11Texture;
            auto result = m_ownerDevice->GetDX11Device()->CreateTexture3D(
                &textureDesc,
                (subresourceData.empty()) ? nullptr : subresourceData.data(),
                dx11Texture.GetAddressOf());

            if (FAILED(result))
            {
                DX_LOG(Fatal, "Texture", "Failed to create 3D texture.");
                return;
            }

            m_dx11Texture = dx11Texture;
        }
        else
        {
            DX_LOG(Fatal, "Utils", "Unknown texture type %d", desc.m_variant);
            return;
        }

        DX_LOG(Verbose, "Texture", "Texture %dD %dx%dx%d, %d mipmaps and %d array created.",
            desc.m_variant, desc.m_size.x, desc.m_size.y, desc.m_size.z, desc.m_mipLevels, desc.m_arraySize);
    }

    Texture::~Texture()
    {
        if (m_dx11Texture)
        {
            DX_LOG(Verbose, "Texture", "Texture %dD %dx%dx%d, %d mipmaps and %d array destroyed.",
                m_desc.m_variant, m_desc.m_size.x, m_desc.m_size.y, m_desc.m_size.z, m_desc.m_mipLevels, m_desc.m_arraySize);
        }
    }

    ComPtr<ID3D11Resource> Texture::GetDX11Texture()
    {
        return m_dx11Texture;
    }
} // namespace DX
