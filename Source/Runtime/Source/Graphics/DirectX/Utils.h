#pragma once

#include <Graphics/Resource/ResourceEnums.h>
#include <Graphics/Resource/Texture/TextureEnums.h>
#include <Graphics/Resource/Buffer/BufferEnums.h>
#include <Graphics/Sampler/SamplerEnums.h>
#include <Graphics/Pipeline/RasterizerState/RasterizerStateEnums.h>
#include <Graphics/Pipeline/BlendState/BlendStateEnums.h>
#include <Graphics/Pipeline/DepthStencilState/DepthStencilStateEnums.h>

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

    D3D11_FILL_MODE ToDX11FillMode(FaceFillMode fillMode);

    D3D11_CULL_MODE ToDX11CullMode(FaceCullMode cullMode);

    D3D11_BLEND ToDX11Blend(Blend blend);

    D3D11_BLEND_OP ToDX11BlendOperation(BlendOperation blendOperation);

    D3D11_STENCIL_OP ToDX11StencilOp(StencilOp stencilOp);

    D3D11_DEPTH_STENCILOP_DESC ToDX11StencipBehaviour(StencilBehaviour stencilBehaviour);
}
