#include <Graphics/Shader/Shader.h>

namespace DX
{
    Shader::Shader(std::shared_ptr<Device> device, const ShaderDesc& desc)
        : DeviceObject(device)
    {
    }
} // namespace DX
