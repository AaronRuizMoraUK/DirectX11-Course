#pragma once

#include <RHI/Resource/ResourceEnums.h>
#include <RHI/Resource/Texture/TextureEnums.h>
#include <RHI/Resource/Buffer/BufferEnums.h>
#include <RHI/Sampler/SamplerEnums.h>
#include <RHI/Pipeline/RasterizerState/RasterizerStateEnums.h>
#include <RHI/Pipeline/BlendState/BlendStateEnums.h>
#include <RHI/Pipeline/DepthStencilState/DepthStencilStateEnums.h>
#include <RHI/Pipeline/InputLayout/InputLayoutEnums.h>

#include <d3d11.h>

namespace DX
{
    D3D11_USAGE ToDX11ResourceUsage(ResourceUsage usage);

    uint32_t ToDX11TextureBindFlags(TextureBindFlags bindFlags);

    uint32_t ToDX11BufferBindFlags(BufferBindFlags bindFlags);

    uint32_t ToDX11ResourceCPUAccess(ResourceCPUAccess cpuAccess);

    DXGI_FORMAT ToDX11ResourceFormat(ResourceFormat format);

    D3D11_FILTER ToDX11FilterSampling(FilterSampling minFilter, FilterSampling magFilter, FilterSampling mipFilter, FilterMode filterMode);

    D3D11_TEXTURE_ADDRESS_MODE ToDX11AddressMode(AddressMode addressMode);

    D3D11_COMPARISON_FUNC ToDX11ComparisonFunction(ComparisonFunction comparisonFunction);

    D3D11_FILL_MODE ToDX11FillMode(FaceFillMode fillMode);

    D3D11_CULL_MODE ToDX11CullMode(FaceCullMode cullMode);

    D3D11_BLEND ToDX11Blend(Blend blend);

    D3D11_BLEND_OP ToDX11BlendOperation(BlendOperation blendOperation);

    uint8_t ToDX11ColorWriteMask(ColorWriteMask colorWriteMask);

    D3D11_STENCIL_OP ToDX11StencilOp(StencilOp stencilOp);

    D3D11_DEPTH_STENCILOP_DESC ToDX11StencipBehaviour(StencilBehaviour stencilBehaviour);

    const char* ToDX11InputSemanticName(InputSemantic semantic, const char* semanticCustomName = nullptr);

    D3D11_PRIMITIVE_TOPOLOGY ToDX11PrimitiveTopology(PrimitiveTopology primitiveTopology, uint32_t controlPointPatchListCount = 0);
}
