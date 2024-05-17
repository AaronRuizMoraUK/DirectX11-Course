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
        if (desc.m_bindFlags == 0)
        {
            DX_LOG(Fatal, "Buffer", "Buffer description with no texture bind flag set.");
            return;
        }

        if ((desc.m_bindFlags & BufferBind_ShaderResource) ||
            (desc.m_bindFlags & BufferBind_ShaderRWResource) ||
            (desc.m_bindFlags & BufferBind_RenderTarget))
        {
            if (desc.m_bufferType == BufferType::None)
            {
                DX_LOG(Fatal, "Buffer", "Buffer description with binding flags ShaderResource, ShaderRWResource or RenderTarget cannot have type None.");
                return;
            }
        }

        // NOTE: In DirectX 12 this was increased to multiples of 256 bytes, equivalent to 4 matrices.
        if ((desc.m_bindFlags & BufferBind_ConstantBuffer) &&
            desc.m_sizeInBytes % 16 != 0)
        {
            DX_LOG(Fatal, "Buffer", "Failed to create buffer. Constant buffers size must be multiples of 16, but passed %d.", desc.m_sizeInBytes);
            return;
        }

        if (desc.m_bufferType == BufferType::Structured &&
            desc.m_structSizeInBytes == 0)
        {
            DX_LOG(Fatal, "Buffer", "Failed to create buffer. Structured buffer require that m_structSizeInBytes must be greater than 0.");
            return;
        }

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = desc.m_sizeInBytes;
        bufferDesc.Usage = ToDX11ResourceUsage(desc.m_usage);
        bufferDesc.BindFlags = ToDX11BufferBindFlags(desc.m_bindFlags);
        bufferDesc.CPUAccessFlags = ToDX11ResourceCPUAccess(desc.m_cpuAccess);
        bufferDesc.MiscFlags = 0;
        switch (desc.m_bufferType)
        {
        case BufferType::Structured:
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            break;
        case BufferType::Raw:
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
        }
        bufferDesc.StructureByteStride = desc.m_structSizeInBytes;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = desc.m_initialData;
        subresourceData.SysMemPitch = 0;
        subresourceData.SysMemSlicePitch = 0;

        auto result = m_ownerDevice->GetDX11Device()->CreateBuffer(
            &bufferDesc,
            (desc.m_initialData) ? &subresourceData : nullptr,
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
