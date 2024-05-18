#pragma once

#include <Graphics/Sampler/SamplerFlags.h>
#include <Math/Vector2.h>
#include <Math/Color.h>

namespace DX
{
    struct SamplerDesc
    {
        // NOTE: If any filter is set to Anisotropic then all of them will be set to anisotropic.
        FilterSampling m_minFilter; // Minification: Filter to apply when texture is bigger than its space in screen
        FilterSampling m_magFilter; // Magnification: Filter to apply when texture is smaller than its space in screen
        FilterSampling m_mipFilter; // Filter to apply between mipmaps
        FilterMode m_filterMode;

        AddressMode m_addressU;
        AddressMode m_addressV;
        AddressMode m_addressW;

        float m_mipBias;
        mathfu::Vector2 m_mipClamp; // 0 is the largest mipmap. For no clamping use DX::NoMipClamping.

        uint32_t m_maxAnisotropy; // Valid values are 1 to 16.

        mathfu::Color m_borderColor;
        ComparisonFunction m_comparisonFunction;
    };
} // namespace DX
