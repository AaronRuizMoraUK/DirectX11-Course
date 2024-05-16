#include <Graphics/Resource/Texture/Texture.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    static const char* GetTextureDimStr(TextureType textureType)
    {
        switch (textureType)
        {
        case TextureType::Texture1D:
            return "1D";
        case TextureType::Texture2D:
            return "2D";
        case TextureType::TextureCube:
            return "Cube";
        case TextureType::Texture3D:
            return "3D";
        default:
            return "Unknown";
        }
    }

    Texture::Texture(Device* device, const TextureDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (desc.m_initialDataIsNativeResource)
        {
            if (!desc.m_initialData)
            {
                DX_LOG(Fatal, "Texture", "Texture description with invalid data.");
                return;
            }

            m_dx11Texture = static_cast<ID3D11Resource*>(desc.m_initialData);
        }
        else if (desc.m_textureType == TextureType::Texture1D)
        {
            D3D11_TEXTURE1D_DESC textureDesc = {};
            textureDesc.Width = desc.m_dimensions.x;
            textureDesc.MipLevels = desc.m_mipCount;
            textureDesc.ArraySize = desc.m_arrayCount;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_initialData)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipCount, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arrayCount, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_initialData);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for (int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_dimensions.x >> mipIndex);
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
        else if (desc.m_textureType == TextureType::Texture2D || desc.m_textureType == TextureType::TextureCube)
        {
            if (desc.m_textureType == TextureType::TextureCube && desc.m_arrayCount % 6 != 0)
            {
                DX_LOG(Fatal, "Texture", "Failed to create Cube texture. Array size must be multiple of 6, but was %d.", desc.m_arrayCount);
                return;
            }

            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = desc.m_dimensions.x;
            textureDesc.Height = desc.m_dimensions.y;
            textureDesc.MipLevels = desc.m_mipCount;
            textureDesc.ArraySize = desc.m_arrayCount;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.SampleDesc.Count = desc.m_sampleCount;
            textureDesc.SampleDesc.Quality = desc.m_sampleQuality;
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;
            if (desc.m_textureType == TextureType::TextureCube)
            {
                textureDesc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
            }

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_initialData)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipCount, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arrayCount, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_initialData);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for (int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_dimensions.x >> mipIndex);
                        const int mipSizeY = std::max<uint32_t>(1, desc.m_dimensions.y >> mipIndex);
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
                DX_LOG(Fatal, "Texture", "Failed to create %s texture.", GetTextureDimStr(desc.m_textureType));
                return;
            }

            m_dx11Texture = dx11Texture;
        }
        else if (desc.m_textureType == TextureType::Texture3D)
        {
            D3D11_TEXTURE3D_DESC textureDesc = {};
            textureDesc.Width = desc.m_dimensions.x;
            textureDesc.Height = desc.m_dimensions.y;
            textureDesc.Depth = desc.m_dimensions.z;
            textureDesc.MipLevels = desc.m_mipCount;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
            textureDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
            textureDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
            textureDesc.MiscFlags = 0;

            std::vector<D3D11_SUBRESOURCE_DATA> subresourceData;
            if (desc.m_initialData)
            {
                uint32_t mipLevels = std::max<uint32_t>(desc.m_mipCount, 1);
                uint32_t arraySize = std::max<uint32_t>(desc.m_arrayCount, 1);

                subresourceData.resize(mipLevels * arraySize);

                std::byte* head = static_cast<std::byte*>(desc.m_initialData);
                for (int arrayIndex = 0; arrayIndex < arraySize; ++arrayIndex)
                {
                    for (int mipIndex = 0; mipIndex < mipLevels; ++mipIndex)
                    {
                        const int index = (arrayIndex * mipLevels) + mipIndex;
                        const int mipSizeX = std::max<uint32_t>(1, desc.m_dimensions.x >> mipIndex);
                        const int mipSizeY = std::max<uint32_t>(1, desc.m_dimensions.y >> mipIndex);
                        const int mipSizeZ = std::max<uint32_t>(1, desc.m_dimensions.z >> mipIndex);
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
            DX_LOG(Fatal, "Utils", "Unknown texture type %d", desc.m_textureType);
            return;
        }

        DX_LOG(Verbose, "Texture", "Texture %s %dx%dx%d, %d mipmaps and %d array created.",
            GetTextureDimStr(desc.m_textureType), desc.m_dimensions.x, desc.m_dimensions.y, desc.m_dimensions.z, desc.m_mipCount, desc.m_arrayCount);
    }

    Texture::~Texture()
    {
        if (m_dx11Texture)
        {
            DX_LOG(Verbose, "Texture", "Texture %s %dx%dx%d, %d mipmaps and %d array destroyed.",
                GetTextureDimStr(m_desc.m_textureType), m_desc.m_dimensions.x, m_desc.m_dimensions.y, m_desc.m_dimensions.z, m_desc.m_mipCount, m_desc.m_arrayCount);
        }
    }

    ComPtr<ID3D11Resource> Texture::GetDX11Texture()
    {
        return m_dx11Texture;
    }
} // namespace DX
