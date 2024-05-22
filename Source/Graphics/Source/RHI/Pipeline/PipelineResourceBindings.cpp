#include <RHI/Pipeline/PipelineResourceBindings.h>
#include <RHI/Pipeline/Pipeline.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Resource/Views/ShaderRWResourceView.h>
#include <RHI/Shader/ShaderBytecode.h>

#include <Log/Log.h>

#include <algorithm>

namespace DX
{
    PipelineResourceBindings::PipelineResourceBindings(
        Pipeline* pipeline, PipelineResourceBindingData&& bindingData)
        : m_pipeline(pipeline)
        , m_bindingData(std::move(bindingData))
    {
    }

    void PipelineResourceBindings::SetConstantBuffer(
        ShaderType shaderType, uint32_t slot, std::shared_ptr<Buffer> buffer)
    {
        if (slot < m_bindingData[shaderType].m_constantBuffers.size())
        {
            m_bindingData[shaderType].m_constantBuffers[slot] = buffer;
        }
        else
        {
            DX_LOG(Warning, "PipelineResourceBindings",
                "No Constant Buffer in %s Shader use register(b%d)",
                ShaderTypeStr(shaderType), slot);
        }
    }

    void PipelineResourceBindings::SetShaderResourceView(
        ShaderType shaderType, uint32_t slot, std::shared_ptr<ShaderResourceView> srv)
    {
        if (slot < m_bindingData[shaderType].m_shaderResourceViews.size())
        {
            m_bindingData[shaderType].m_shaderResourceViews[slot] = srv;
        }
        else
        {
            DX_LOG(Warning, "PipelineResourceBindings",
                "No Texture or Buffer in %s Shader use register(t%d)",
                ShaderTypeStr(shaderType), slot);
        }
    }
    void PipelineResourceBindings::SetShaderRWResourceView(
        ShaderType shaderType, uint32_t slot, std::shared_ptr<ShaderRWResourceView> srwrv)
    {
        if (slot < m_bindingData[shaderType].m_shaderRWResourceViews.size())
        {
            m_bindingData[shaderType].m_shaderRWResourceViews[slot] = srwrv;
        }
        else
        {
            DX_LOG(Warning, "PipelineResourceBindings",
                "No read-write Texture or Buffer in %s Shader use register(u%d)",
                ShaderTypeStr(shaderType), slot);
        }
    }

    void PipelineResourceBindings::SetSampler(
        ShaderType shaderType, uint32_t slot, std::shared_ptr<Sampler> sampler)
    {
        if (slot < m_bindingData[shaderType].m_samplers.size())
        {
            m_bindingData[shaderType].m_samplers[slot] = sampler;
        }
        else
        {
            DX_LOG(Warning, "PipelineResourceBindings",
                "No Sampler in %s Shader use register(u%d)",
                ShaderTypeStr(shaderType), slot);
        }
    }

    void PipelineResourceBindings::SetConstantBuffer(
        ShaderType shaderType, const std::string& name, std::shared_ptr<Buffer> buffer)
    {
        const ShaderResourceLayout* shaderResourceLayout = m_pipeline->GetShaderResourceLayout(shaderType);
        if (!shaderResourceLayout)
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Pipeline doesn't use %s Shader", ShaderTypeStr(shaderType));
            return;
        }

        auto it = std::ranges::find_if(shaderResourceLayout->m_constantBuffers, 
            [&name](const ShaderResourceInfo& resourceInfo)
            {
                return resourceInfo.m_name == name;
            });

        if (it == shaderResourceLayout->m_constantBuffers.end())
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Constant Buffer '%s' not found in %s Shader", 
                name.c_str(), ShaderTypeStr(shaderType));
            return;
        }

        SetConstantBuffer(shaderType, it->m_startSlot, buffer);
    }

    void PipelineResourceBindings::SetShaderResourceView(
        ShaderType shaderType, const std::string& name, std::shared_ptr<ShaderResourceView> srv)
    {
        const ShaderResourceLayout* shaderResourceLayout = m_pipeline->GetShaderResourceLayout(shaderType);
        if (!shaderResourceLayout)
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Pipeline doesn't use %s Shader", ShaderTypeStr(shaderType));
            return;
        }

        auto it = std::ranges::find_if(shaderResourceLayout->m_shaderResourceViews,
            [&name](const ShaderResourceInfo& resourceInfo)
            {
                return resourceInfo.m_name == name;
            });

        if (it == shaderResourceLayout->m_shaderResourceViews.end())
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Texture or Buffer '%s' not found in %s Shader",
                name.c_str(), ShaderTypeStr(shaderType));
            return;
        }

        SetShaderResourceView(shaderType, it->m_startSlot, srv);
    }

    void PipelineResourceBindings::SetShaderRWResourceView(
        ShaderType shaderType, const std::string& name, std::shared_ptr<ShaderRWResourceView> srwrv)
    {
        const ShaderResourceLayout* shaderResourceLayout = m_pipeline->GetShaderResourceLayout(shaderType);
        if (!shaderResourceLayout)
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Pipeline doesn't use %s Shader", ShaderTypeStr(shaderType));
            return;
        }

        auto it = std::ranges::find_if(shaderResourceLayout->m_shaderRWResourceViews,
            [&name](const ShaderResourceInfo& resourceInfo)
            {
                return resourceInfo.m_name == name;
            });

        if (it == shaderResourceLayout->m_shaderRWResourceViews.end())
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Texture or Buffer '%s' not found in %s Shader",
                name.c_str(), ShaderTypeStr(shaderType));
            return;
        }

        SetShaderRWResourceView(shaderType, it->m_startSlot, srwrv);
    }

    void PipelineResourceBindings::SetSampler(
        ShaderType shaderType, const std::string& name, std::shared_ptr<Sampler> sampler)
    {
        const ShaderResourceLayout* shaderResourceLayout = m_pipeline->GetShaderResourceLayout(shaderType);
        if (!shaderResourceLayout)
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Pipeline doesn't use %s Shader", ShaderTypeStr(shaderType));
            return;
        }

        auto it = std::ranges::find_if(shaderResourceLayout->m_samplers,
            [&name](const ShaderResourceInfo& resourceInfo)
            {
                return resourceInfo.m_name == name;
            });

        if (it == shaderResourceLayout->m_samplers.end())
        {
            DX_LOG(Warning, "PipelineResourceBindings", "Sampler '%s' not found in %s Shader",
                name.c_str(), ShaderTypeStr(shaderType));
            return;
        }

        SetSampler(shaderType, it->m_startSlot, sampler);
    }

    const Pipeline* PipelineResourceBindings::GetPipeline() const
    {
        return m_pipeline;
    }

    const PipelineResourceBindingData& PipelineResourceBindings::GetBindingData() const
    {
        return m_bindingData;
    }
} // namespace DX
