#include <Graphics/Resource/Buffer/Buffer.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    Buffer::Buffer(Device* device, const BufferDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = desc.m_sizeInBytes;
        bufferDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
        bufferDesc.BindFlags = ToDX11ResourceBindFlag(desc.m_bindFlag);
        bufferDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0; // TODO

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = desc.m_initialData;
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;

        auto result = m_ownerDevice->GetDX11Device()->CreateBuffer(
            &bufferDesc,
            (desc.m_initialData) ?  &subresourceData : nullptr,
            m_dx11Buffer.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Fatal, "Buffer", "Failed to create buffer.");
            return;
        }

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
