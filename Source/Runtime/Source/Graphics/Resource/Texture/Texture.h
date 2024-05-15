#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Texture/TextureDesc.h>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11Texture1D;
class ID3D11Texture2D;
class ID3D11Texture3D;

#include <variant>

namespace DX
{
    class Texture : public DeviceObject
    {
    public:
        Texture(Device* device, const TextureDesc& desc);
        ~Texture() = default;

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Texture; }

    private:
        using DX11Texture = std::variant<ComPtr<ID3D11Texture1D>, ComPtr<ID3D11Texture2D>, ComPtr<ID3D11Texture3D>>;
        DX11Texture m_dx11Texture;
    };
} // namespace DX
