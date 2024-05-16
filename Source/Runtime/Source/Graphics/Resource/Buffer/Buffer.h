#pragma once

#include <Graphics/DeviceObject/DeviceObject.h>
#include <Graphics/Resource/Buffer/BufferDesc.h>

#include <Graphics/DirectX/ComPtr.h>
class ID3D11Buffer;

namespace DX
{
    class Buffer : public DeviceObject
    {
    public:
        Buffer(Device* device, const BufferDesc& desc);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Buffer; }

        const BufferDesc& GetBufferDesc() const { return m_desc; }

        ComPtr<ID3D11Buffer> GetDX11Buffer();

    private:
        BufferDesc m_desc;

    private:
        ComPtr<ID3D11Buffer> m_dx11Buffer;
    };
} // namespace DX
