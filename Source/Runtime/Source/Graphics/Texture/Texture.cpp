#include <Graphics/Texture/Texture.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    // TODO: Replace all object's comptr textures with Textures
    // TODO: Add a constructor passing comptr texture directly (useful for Swap Chain)
    // TODO: Move initialization of all objects out of constructor
    Texture::Texture(Device* device, const TextureDesc& desc)
        : DeviceObject(device)
    {
        if (desc.m_type == TextureType::Texture1D)
        {
            D3D11_TEXTURE1D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.ArraySize = desc.m_arraySize;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = D3D11_USAGE_IMMUTABLE; // TODO
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // TODO
            textureDesc.CPUAccessFlags = 0; // TODO
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
        else if (desc.m_type == TextureType::Texture2D)
        {
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.Height = desc.m_size.y;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.ArraySize = desc.m_arraySize;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.SampleDesc.Count = desc.m_sampleCount;
            textureDesc.SampleDesc.Quality = desc.m_sampleQuality;
            textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
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
        else if (desc.m_type == TextureType::Texture3D)
        {
            D3D11_TEXTURE3D_DESC textureDesc = {};
            textureDesc.Width = desc.m_size.x;
            textureDesc.Height = desc.m_size.y;
            textureDesc.Depth = desc.m_size.z;
            textureDesc.MipLevels = desc.m_mipLevels;
            textureDesc.Format = ToDX11ResourceFormat(desc.m_format);
            textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
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
            DX_LOG(Fatal, "Utils", "Unknown texture type %d", desc.m_type);
        }
    }
} // namespace DX
