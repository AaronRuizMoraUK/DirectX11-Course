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
        if (m_desc.m_minFilter == FilterSampling::Anisotropic ||
            m_desc.m_magFilter == FilterSampling::Anisotropic || 
            m_desc.m_mipFilter == FilterSampling::Anisotropic)
        {
            if (m_desc.m_minFilter != m_desc.m_magFilter ||
                m_desc.m_minFilter != m_desc.m_mipFilter)
            {
                m_desc.m_minFilter = FilterSampling::Anisotropic;
                m_desc.m_magFilter = FilterSampling::Anisotropic;
                m_desc.m_mipFilter = FilterSampling::Anisotropic;
                DX_LOG(Warning, "Sampler", "All filters will be set to Anisotropic since one of them was set to Anisotropic.");
            }

            if (m_desc.m_maxAnisotropy < 1 || m_desc.m_maxAnisotropy > 16)
            {
                DX_LOG(Fatal, "Sampler", "Invalid max anisotropy value %d. It needs to be between 1 and 16.", m_desc.m_maxAnisotropy);
                return;
            }
        }

        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = ToDX11FilterSampling(m_desc.m_minFilter, m_desc.m_magFilter, m_desc.m_mipFilter, m_desc.m_filterMode);
        samplerDesc.AddressU = ToDX11AddressMode(m_desc.m_addressU);
        samplerDesc.AddressV = ToDX11AddressMode(m_desc.m_addressV);
        samplerDesc.AddressW = ToDX11AddressMode(m_desc.m_addressW);
        samplerDesc.MipLODBias = m_desc.m_mipBias;
        samplerDesc.MaxAnisotropy = m_desc.m_maxAnisotropy;
        samplerDesc.ComparisonFunc = ToDX11ComparisonFunction(m_desc.m_comparisonFunction);
        for (int i = 0; i < m_desc.m_borderColor.Dims; ++i)
        {
            samplerDesc.BorderColor[i] = m_desc.m_borderColor[i];
        }
        samplerDesc.MinLOD = m_desc.m_mipClamp.x;
        samplerDesc.MaxLOD = m_desc.m_mipClamp.y;

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
