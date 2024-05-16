#include <Graphics/Resource/Buffer/Buffer.h>

#include <d3d11.h>

namespace DX
{
    Buffer::Buffer(Device* device, const BufferDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        DX_LOG(Verbose, "Buffer", "Graphics buffer created.");
    }

    Buffer::~Buffer()
    {
        if (m_dx11Buffer)
        {
            DX_LOG(Verbose, "Buffer", "Graphics buffer destroyed.");
        }
    }

    ComPtr<ID3D11Buffer> Buffer::GetDX11Buffer()
    {
        return m_dx11Buffer;
    }
} // namespace DX
