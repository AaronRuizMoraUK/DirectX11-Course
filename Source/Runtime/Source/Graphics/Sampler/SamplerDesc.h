#pragma once

#include <Graphics/Sampler/SamplerFlags.h>
#include <Math/Vector2.h>
#include <Math/Color.h>

namespace DX
{
    struct SamplerDesc
    {
        FilterSampling m_minFilter; // Minification: Filter to apply when texture is bigger than its space in screen
        FilterSampling m_magFilter; // Magnification: Filter to apply when texture is smaller than its space in screen
        FilterSampling m_mipFilter; // Filter to apply between mipmaps
        FilterMode m_filterMode;

        AddressMode m_addressU;
        AddressMode m_addressV;
        AddressMode m_addressW;

        float m_mipBias;
        mathfu::Vector2 m_mipClamp; // 0 is the largest mipmap. No clamping would be [0, FLT_MAX].

        uint32_t m_maxAnisotropy; // Valid values are 1 to 16.

        mathfu::Color m_borderColor;
        ComparisonFunction m_comparisonFunction;
    };
} // namespace DX
