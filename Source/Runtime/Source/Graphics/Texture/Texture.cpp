#include <Graphics/Texture/Texture.h>

namespace DX
{
    Texture::Texture(std::shared_ptr<Device> device, const TextureDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
