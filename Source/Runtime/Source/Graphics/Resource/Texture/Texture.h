#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Texture/TextureDesc.h>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11Resource;

namespace DX
{
    class Texture : public DeviceObject
    {
    public:
        Texture(Device* device, const TextureDesc& desc);
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Texture; }

        const TextureDesc& GetTextureDesc() const { return m_desc; }

        ComPtr<ID3D11Resource> GetDX11Texture();

    private:
        TextureDesc m_desc;

    private:
        ComPtr<ID3D11Resource> m_dx11Texture;
    };
} // namespace DX
