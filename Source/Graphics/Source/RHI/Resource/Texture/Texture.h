#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Texture/TextureDesc.h>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11Resource;

namespace DX
{
    //------------------------------------------------------------------------
    // Texture Format
    // 
    // Texture format is set explicitly inside the resource.
    // All its usages are views where the format must be specified too.
    // The texture format and the view format must be compatible.
    //------------------------------------------------------------------------

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
