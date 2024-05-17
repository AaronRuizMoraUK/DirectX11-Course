#pragma once

#include <Graphics/Resource/ResourceFlags.h>
#include <Graphics/Resource/Texture/TextureDesc.h>
#include <Graphics/Resource/Buffer/BufferDesc.h>

#include <d3d11.h>

namespace DX
{
    D3D11_USAGE ToDX11ResourceUsage(ResourceUsage usage);

    uint32_t ToDX11TextureBindFlags(TextureBindFlags bindFlag);

    uint32_t ToDX11BufferBindFlags(BufferBindFlags bindFlag);

    uint32_t ToDX11ResourceCPUAccess(ResourceCPUAccess cpuAccess);

    DXGI_FORMAT ToDX11ResourceFormat(ResourceFormat format);
}
