#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Views/ShaderResourceViewDesc.h>

#include <RHI/DirectX/ComPtr.h>
class ID3D11ShaderResourceView;

namespace DX
{
    class ShaderResourceView : public DeviceObject
    {
    public:
        ShaderResourceView(Device* device, const ShaderResourceViewDesc& desc);
        ~ShaderResourceView();

        ShaderResourceView(const ShaderResourceView&) = delete;
        ShaderResourceView& operator=(const ShaderResourceView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::ShaderResourceView; }

        const ShaderResourceViewDesc& GetShaderResourceViewDesc() const { return m_desc; }

        ComPtr<ID3D11ShaderResourceView> GetDX11ShaderResourceView();

    private:
        ShaderResourceViewDesc m_desc;

    private:
        ComPtr<ID3D11ShaderResourceView> m_dx11ShaderResourceView;
    };
} // namespace DX
