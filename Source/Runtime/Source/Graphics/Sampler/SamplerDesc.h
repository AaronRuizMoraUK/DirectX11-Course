#pragma once

#include <Graphics/Sampler/SamplerFlags.h>
#include <Math/Color.h>

namespace DX
{
    struct SamplerDesc
    {
        FilterSampling m_minFilter;
        FilterSampling m_magFilter;
        FilterSampling m_mipFilter;
        FilterMode m_filterMode;

        AddressMode m_addressU;
        AddressMode m_addressV;
        AddressMode m_addressW;

        float m_mipBias;
        float m_minMip; // 0 is the largest mipmap
        float m_maxMip;

        uint32_t m_maxAnisotropy; // Valid values are 1 to 16.

        mathfu::Color m_borderColor;
    };
} // namespace DX
