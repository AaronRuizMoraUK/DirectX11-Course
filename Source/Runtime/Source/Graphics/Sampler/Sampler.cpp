#include <Graphics/Sampler/Sampler.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

namespace DX
{
    Sampler::Sampler(Device* device, const SamplerDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (desc.m_maxAnisotropy < 1 || desc.m_maxAnisotropy > 16)
        {
            DX_LOG(Fatal, "Sampler", "Invalid max anisotropy value %d. It needs to be between 1 and 16.", desc.m_maxAnisotropy);
            return;
        }

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = ToDX11FilterSampling(desc.m_minFilter, desc.m_magFilter, desc.m_mipFilter, desc.m_filterMode);
        samplerDesc.AddressU = ToDX11AddressMode(desc.m_addressU);
        samplerDesc.AddressV = ToDX11AddressMode(desc.m_addressV);
        samplerDesc.AddressW = ToDX11AddressMode(desc.m_addressW);
        samplerDesc.MipLODBias = desc.m_mipBias;
        samplerDesc.MaxAnisotropy = desc.m_maxAnisotropy;
        samplerDesc.ComparisonFunc = ToDX11ComparisonFunction(desc.m_comparisonFunction);
        for (int i = 0; i < desc.m_borderColor.Dims; ++i)
        {
            samplerDesc.BorderColor[i] = desc.m_borderColor[i];
        }
        samplerDesc.MinLOD = desc.m_mipClamp.x;
        samplerDesc.MaxLOD = desc.m_mipClamp.y;

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
