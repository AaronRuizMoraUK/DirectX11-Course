#include <Graphics/Sampler/Sampler.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

namespace DX
{
    Sampler::Sampler(Device* device, const SamplerDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        // TODO
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDesc.MipLODBias = 0.0f;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        for (int i = 0; i < 4; ++i)
        {
            samplerDesc.BorderColor[i] = 0.0f;
        }
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        auto result =m_ownerDevice->GetDX11Device()->CreateSamplerState(&samplerDesc, m_dx11Sampler.GetAddressOf());

        if (FAILED(result))
        {
            DX_LOG(Fatal, "Sampler", "Failed to create sampler.");
            return;
        }

        DX_LOG(Verbose, "Sampler", "Sampler created.");
    }

    Sampler::~Sampler()
    {
        if (m_dx11Sampler)
        {
            DX_LOG(Verbose, "Sampler", "Sampler destroyed.");
        }
    }

    ComPtr<ID3D11SamplerState> Sampler::GetDX11Sampler()
    {
        return m_dx11Sampler;
    }
} // namespace DX
