#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Resource/Views/ShaderRWResourceViewDesc.h>

#include <RHI/DirectX/ComPtr.h>
class ID3D11UnorderedAccessView;

namespace DX
{
    class ShaderRWResourceView : public DeviceObject
    {
    public:
        ShaderRWResourceView(Device* device, const ShaderRWResourceViewDesc& desc);
        ~ShaderRWResourceView();

        ShaderRWResourceView(const ShaderRWResourceView&) = delete;
        ShaderRWResourceView& operator=(const ShaderRWResourceView&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::ShaderRWResourceView; }

        const ShaderRWResourceViewDesc& GetShaderRWResourceViewDesc() const { return m_desc; }

        ComPtr<ID3D11UnorderedAccessView> GetDX11UnorderedAccessView();

    private:
        ShaderRWResourceViewDesc m_desc;

    private:
        ComPtr<ID3D11UnorderedAccessView> m_dx11UnorderedAccessView;
    };
} // namespace DX
