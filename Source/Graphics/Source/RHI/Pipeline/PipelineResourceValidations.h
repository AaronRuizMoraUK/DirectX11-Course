#pragma once

#include <RHI/Shader/ShaderEnums.h>
#include <RHI/Shader/ShaderResourceLayout.h>

#include <vector>
#include <memory>

namespace DX
{
    class Buffer;
    class Sampler;
    class ShaderResourceView;
    class ShaderRWResourceView;

    void ValidateConstantBufferBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& constantBuffersInfo,
        const std::vector<std::shared_ptr<Buffer>>& constantBuffers);


    void ValidateShaderResourceViewBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& shaderResourceViewsInfo,
        const std::vector<std::shared_ptr<ShaderResourceView>>& shaderResourceViews);

    void ValidateShaderRWResourceViewBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& shaderRWResourceViewsInfo,
        const std::vector<std::shared_ptr<ShaderRWResourceView>>& shaderRWResourceViews);

    void ValidateSamplersBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& samplersInfo,
        const std::vector<std::shared_ptr<Sampler>>& samplers);
} // namespace DX
