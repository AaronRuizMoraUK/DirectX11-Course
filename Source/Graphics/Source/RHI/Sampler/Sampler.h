#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Sampler/SamplerDesc.h>

#include <RHI/DirectX/ComPtr.h>
class ID3D11SamplerState;

namespace DX
{
    class Sampler : public DeviceObject
    {
    public:
        Sampler(Device* device, const SamplerDesc& desc);
        ~Sampler();

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Sampler; }

        SamplerDesc GetSamplerDesc() const { return m_desc; }

        ComPtr<ID3D11SamplerState> GetDX11Sampler();

    private:
        SamplerDesc m_desc;

    private:
        ComPtr<ID3D11SamplerState> m_dx11Sampler;
    };
} // namespace DX
