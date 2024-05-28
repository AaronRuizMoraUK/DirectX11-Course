#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Buffer/BufferDesc.h>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11Buffer;

namespace DX
{
    //------------------------------------------------------------------------
    // Buffer Format
    // 
    // Buffer format is implicit and not set inside the description.
    // The format of the buffer is set later depending on what's used for (binding):
    // 
    // - As VertexBuffer: format is specified with an InputLayout, which is set with DeviceContext->IASetInputLayout() call.
    // - As IndexBuffer: format is specified in DeviceContext->IASetIndexBuffer() call.
    // - As ConstantBuffer: format is expected to match the structure of the constant buffer in the shader.
    // - As ShaderResource, ShaderRWResource or RenderTarget: format is specified in their respective view description.
    // - As StreamOutput: format is always 32-bit values coming from the output declarations of the shader stages.
    //------------------------------------------------------------------------

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
