#pragma once

#include <Graphics/Resource/ResourceFlags.h>
#include <Graphics/Resource/Texture/TextureFlags.h>
#include <Graphics/Resource/Buffer/BufferFlags.h>
#include <Graphics/Sampler/SamplerFlags.h>

#include <d3d11.h>

namespace DX
{
    D3D11_USAGE ToDX11ResourceUsage(ResourceUsage usage);

    uint32_t ToDX11TextureBindFlags(TextureBindFlags bindFlag);

    uint32_t ToDX11BufferBindFlags(BufferBindFlags bindFlag);

    uint32_t ToDX11ResourceCPUAccess(ResourceCPUAccess cpuAccess);

    DXGI_FORMAT ToDX11ResourceFormat(ResourceFormat format);

    D3D11_FILTER ToDX11FilterSampling(FilterSampling minFilter, FilterSampling magFilter, FilterSampling mipFilter, FilterMode filterMode);

    D3D11_TEXTURE_ADDRESS_MODE ToDX11AddressMode(AddressMode addressMode);

    D3D11_COMPARISON_FUNC ToDX11ComparisonFunction(ComparisonFunction comparisonFunction);
}
