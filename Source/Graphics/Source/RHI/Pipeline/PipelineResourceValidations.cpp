#include <RHI/Pipeline/PipelineResourceValidations.h>

#include <RHI/Pipeline/Pipeline.h>
#include <RHI/Pipeline/PipelineResourceBindings.h>
#include <RHI/Shader/ShaderEnums.h>
#include <RHI/Shader/ShaderResourceLayout.h>
#include <RHI/Sampler/Sampler.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Texture/Texture.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Resource/Views/ShaderRWResourceView.h>

#include <Log/Log.h>
#include <Debug/Debug.h>

namespace DX
{
    static void ValidateConstantBufferBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& constantBuffersInfo,
        const std::vector<std::shared_ptr<Buffer>>& constantBuffers)
    {
        for (const auto& cbInfo : constantBuffersInfo)
        {
            for (uint32_t slot = cbInfo.m_startSlot; slot < cbInfo.m_startSlot + cbInfo.m_slotCount; ++slot)
            {
                const Buffer* cb = constantBuffers[slot].get();
                if (!cb)
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Missing to set a Constant Buffer in slot %d (name '%s') for %s Shader %s.",
                        slot, cbInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    continue;
                }

                // The Buffer set in binding data must be a constant buffer
                const BufferDesc& bufferDesc = cb->GetBufferDesc();
                if (!(bufferDesc.m_bindFlags & BufferBind_ConstantBuffer))
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Buffer set in slot %d (name '%s') for %s Shader %s is not a Constant Buffer.",
                        slot, cbInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    continue;
                }
            }
        }
    }

    static void ValidateShaderResourceViewBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& shaderResourceViewsInfo,
        const std::vector<std::shared_ptr<ShaderResourceView>>& shaderResourceViews)
    {
        for (const auto& srvInfo : shaderResourceViewsInfo)
        {
            for (uint32_t slot = srvInfo.m_startSlot; slot < srvInfo.m_startSlot + srvInfo.m_slotCount; ++slot)
            {
                const ShaderResourceView* srv = shaderResourceViews[slot].get();
                if (!srv)
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Missing to set a Shader Resource View in slot %d (name '%s'). %s Shader %s.",
                        slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    continue;
                }

                // Is the View holding a buffer?
                if (srvInfo.m_bufferType != BufferType::None)
                {
                    // Buffer types from layout and set int binding data must match
                    if (auto* buffer = std::get_if<std::shared_ptr<Buffer>>(&srv->GetShaderResourceViewDesc().m_resource))
                    {
                        const BufferDesc& bufferDesc = buffer->get()->GetBufferDesc();
                        if (srvInfo.m_bufferType != bufferDesc.m_bufferType)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to the expected Buffer type. Expected: %d Set: %d.",
                                slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str(),
                                srvInfo.m_bufferType, bufferDesc.m_bufferType);
                        }
                    }
                    else
                    {
                        DX_LOG(Error, "PipelineResourceValidations",
                            "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Buffer.",
                            slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    }
                }
                // Is the View holding a texture?
                else if (srvInfo.m_textureType != TextureType::Unknown)
                {
                    // Texture types from layout and set int binding data must match
                    if (auto* texture = std::get_if<std::shared_ptr<Texture>>(&srv->GetShaderResourceViewDesc().m_resource))
                    {
                        const TextureDesc& textureDesc = texture->get()->GetTextureDesc();
                        if (srvInfo.m_textureType != textureDesc.m_textureType)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to the expected Texture type. Expected: %d Set: %d.",
                                slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str(),
                                srvInfo.m_textureType, textureDesc.m_textureType);
                        }

                        if ((srvInfo.m_textureSubTypeFlags & TextureSubType_Array) &&
                            textureDesc.m_arrayCount < 2)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture Array.",
                                slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                        }

                        if ((srvInfo.m_textureSubTypeFlags & TextureSubType_Multisample) &&
                            textureDesc.m_sampleCount < 2)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture with multisampling.",
                                slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                        }
                    }
                    else
                    {
                        DX_LOG(Error, "PipelineResourceValidations",
                            "Shader Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture.",
                            slot, srvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    }
                }
                else
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Invalid Shader Resource View info from layout (). It doesn't hold a texture or a buffer. %s Shader %s.",
                        ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                }
            }
        }
    }

    static void ValidateShaderRWResourceViewBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& shaderRWResourceViewsInfo,
        const std::vector<std::shared_ptr<ShaderRWResourceView>>& shaderRWResourceViews)
    {
        for (const auto& srwrvInfo : shaderRWResourceViewsInfo)
        {
            for (uint32_t slot = srwrvInfo.m_startSlot; slot < srwrvInfo.m_startSlot + srwrvInfo.m_slotCount; ++slot)
            {
                const ShaderRWResourceView* srwrv = shaderRWResourceViews[slot].get();
                if (!srwrv)
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Missing to set a Shader RW Resource View in slot %d (name '%s') for %s Shader %s.",
                        slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    continue;
                }

                // Is the View holding a buffer?
                if (srwrvInfo.m_bufferType != BufferType::None)
                {
                    // Buffer types from layout and set int binding data must match
                    if (auto* buffer = std::get_if<std::shared_ptr<Buffer>>(&srwrv->GetShaderRWResourceViewDesc().m_resource))
                    {
                        const BufferDesc& bufferDesc = buffer->get()->GetBufferDesc();
                        if (srwrvInfo.m_bufferType != bufferDesc.m_bufferType)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to the expected Buffer type. Expected: %d Set: %d.",
                                slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str(),
                                srwrvInfo.m_bufferType, bufferDesc.m_bufferType);
                        }
                    }
                    else
                    {
                        DX_LOG(Error, "PipelineResourceValidations",
                            "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Buffer.",
                            slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    }
                }
                // Is the View holding a texture?
                else if (srwrvInfo.m_textureType != TextureType::Unknown)
                {
                    // Texture types from layout and set int binding data must match
                    if (auto* texture = std::get_if<std::shared_ptr<Texture>>(&srwrv->GetShaderRWResourceViewDesc().m_resource))
                    {
                        const TextureDesc& textureDesc = texture->get()->GetTextureDesc();
                        if (srwrvInfo.m_textureType != textureDesc.m_textureType)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to the expected Texture type. Expected: %d Set: %d.",
                                slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str(),
                                srwrvInfo.m_textureType, textureDesc.m_textureType);
                        }

                        if ((srwrvInfo.m_textureSubTypeFlags & TextureSubType_Array) &&
                            textureDesc.m_arrayCount < 2)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture Array.",
                                slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                        }

                        if ((srwrvInfo.m_textureSubTypeFlags & TextureSubType_Multisample) &&
                            textureDesc.m_sampleCount < 2)
                        {
                            DX_LOG(Error, "PipelineResourceValidations",
                                "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture with multisampling.",
                                slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                        }
                    }
                    else
                    {
                        DX_LOG(Error, "PipelineResourceValidations",
                            "Shader RW Resource View set in slot %d (name '%s') for %s Shader %s does not point to a Texture.",
                            slot, srwrvInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    }
                }
                else
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Invalid Shader RW Resource View info from layout (). It doesn't hold a texture or a buffer. %s Shader %s.",
                        ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                }
            }
        }
    }

    static void ValidateSamplersBindings(const ShaderInfo& shaderInfo,
        const std::vector<ShaderResourceInfo>& samplersInfo,
        const std::vector<std::shared_ptr<Sampler>>& samplers)
    {
        for (const auto& samplerInfo : samplersInfo)
        {
            for (uint32_t slot = samplerInfo.m_startSlot; slot < samplerInfo.m_startSlot + samplerInfo.m_slotCount; ++slot)
            {
                const Sampler* sampler = samplers[slot].get();
                if (!sampler)
                {
                    DX_LOG(Error, "PipelineResourceValidations",
                        "Missing to set a Sampler in slot %d (name '%s') for %s Shader %s.",
                        slot, samplerInfo.m_name.c_str(), ShaderTypeStr(shaderInfo.m_shaderType), shaderInfo.m_name.c_str());
                    continue;
                }
            }
        }
    }

    void ValidatePipelineResourceBindings(const PipelineResourceBindings& resources)
    {
        for (int i = 0; i < ShaderType_Count; ++i)
        {
            const ShaderType shaderType = static_cast<ShaderType>(i);

            const ShaderResourceLayout* shaderResourceLayout =
                resources.GetPipeline()->GetShaderResourceLayout(shaderType);

            const ShaderResourceBindingData& shaderBindingData = resources.GetBindingData()[shaderType];

            if (!shaderResourceLayout)
            {
                DX_ASSERT(shaderBindingData.m_constantBuffers.empty(), "BindResources",
                    "Shader binding data should have no constant buffers for %s Shader", ShaderTypeStr(shaderType));
                DX_ASSERT(shaderBindingData.m_shaderResourceViews.empty(), "BindResources",
                    "Shader binding data should have no shader resource views in %s Shader", ShaderTypeStr(shaderType));
                DX_ASSERT(shaderBindingData.m_shaderRWResourceViews.empty(), "BindResources",
                    "Shader binding data should have no shader RW resource views for %s Shader", ShaderTypeStr(shaderType));
                DX_ASSERT(shaderBindingData.m_samplers.empty(), "BindResources",
                    "Shader binding data should have no samplers for %s Shader", ShaderTypeStr(shaderType));
                continue;
            }

            const ShaderInfo& shaderInfo =
                resources.GetPipeline()->GetPipelineShader(shaderType)->GetShaderDesc().m_shaderInfo;

            ValidateConstantBufferBindings(
                shaderInfo, shaderResourceLayout->m_constantBuffers, shaderBindingData.m_constantBuffers);
            ValidateShaderResourceViewBindings(
                shaderInfo, shaderResourceLayout->m_shaderResourceViews, shaderBindingData.m_shaderResourceViews);
            ValidateShaderRWResourceViewBindings(
                shaderInfo, shaderResourceLayout->m_shaderRWResourceViews, shaderBindingData.m_shaderRWResourceViews);
            ValidateSamplersBindings(
                shaderInfo, shaderResourceLayout->m_samplers, shaderBindingData.m_samplers);
        }
    }
} // namespace DX
