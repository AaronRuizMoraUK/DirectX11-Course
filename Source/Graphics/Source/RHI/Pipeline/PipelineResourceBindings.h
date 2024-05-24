#pragma once

#include <RHI/Shader/ShaderEnums.h>

#include <vector>
#include <array>
#include <memory>

namespace DX
{
    class Pipeline;
    class Buffer;
    class ShaderResourceView;
    class ShaderRWResourceView;
    class Sampler;

    struct ShaderResourceBindingData
    {
        std::vector<std::shared_ptr<Buffer>> m_constantBuffers;
        std::vector<std::shared_ptr<ShaderResourceView>> m_shaderResourceViews;
        std::vector<std::shared_ptr<ShaderRWResourceView>> m_shaderRWResourceViews;
        std::vector<std::shared_ptr<Sampler>> m_samplers;
    };

    using PipelineResourceBindingData = std::array<ShaderResourceBindingData, ShaderType_Count>;

    class PipelineResourceBindings
    {
    public:
        PipelineResourceBindings() = default;
        PipelineResourceBindings(Pipeline* pipeline, PipelineResourceBindingData&& bindingData);
        ~PipelineResourceBindings() = default;

        // Set resources using slot
        void SetConstantBuffer(ShaderType shaderType, uint32_t slot, std::shared_ptr<Buffer> buffer);
        void SetShaderResourceView(ShaderType shaderType, uint32_t slot, std::shared_ptr<ShaderResourceView> srv);
        void SetShaderRWResourceView(uint32_t slot, std::shared_ptr<ShaderRWResourceView> srwrv);
        void SetSampler(ShaderType shaderType, uint32_t slot, std::shared_ptr<Sampler> sampler);

        // Set resources using its variable name in the shader
        void SetConstantBuffer(ShaderType shaderType, const std::string& name, std::shared_ptr<Buffer> buffer);
        void SetShaderResourceView(ShaderType shaderType, const std::string& name, std::shared_ptr<ShaderResourceView> srv);
        void SetShaderRWResourceView(const std::string& name, std::shared_ptr<ShaderRWResourceView> srwrv);
        void SetSampler(ShaderType shaderType, const std::string& name, std::shared_ptr<Sampler> sampler);

        const Pipeline* GetPipeline() const;

        const PipelineResourceBindingData& GetBindingData() const;

    private:
        Pipeline* m_pipeline;

        PipelineResourceBindingData m_bindingData;
    };
} // namespace DX
