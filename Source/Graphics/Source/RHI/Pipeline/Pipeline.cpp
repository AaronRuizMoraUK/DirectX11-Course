#include <RHI/Pipeline/Pipeline.h>

#include <RHI/Device/Device.h>
#include <RHI/Shader/ShaderBytecode.h>
#include <RHI/Pipeline/InputLayout/InputLayout.h>
#include <RHI/Pipeline/RasterizerState/RasterizerState.h>
#include <RHI/Pipeline/BlendState/BlendState.h>
#include <RHI/Pipeline/DepthStencilState/DepthStencilState.h>
#include <Log/Log.h>
#include <Debug/Debug.h>

#include <ranges>

#include <d3d11.h>
#include <RHI/DirectX/Utils.h>

#pragma DX_DISABLE_WARNING(4267, "")

namespace DX
{
    Pipeline::Pipeline(Device* device, const PipelineDesc& desc)
        : DeviceObject(device)
        , m_desc(desc)
    {
        if (!CreateInputLayout())
        {
            DX_LOG(Fatal, "Pipeline", "Failed to create input layout in pipeline.");
            return;
        }

        if (!CreateRasterizerState())
        {
            DX_LOG(Fatal, "Pipeline", "Failed to create rasterizer state in pipeline.");
            return;
        }

        if (!CreateBlendState())
        {
            DX_LOG(Fatal, "Pipeline", "Failed to create blend state in pipeline.");
            return;
        }

        if (!CreateDepthStencilState())
        {
            DX_LOG(Fatal, "Pipeline", "Failed to create depth stencil state in pipeline.");
            return;
        }

        if (!CreatePipelineResourceBindings())
        {
            DX_LOG(Fatal, "Pipeline", "Failed to create pipeline resource bindings.");
            return;
        }

        DX_LOG(Verbose, "Pipeline", "Graphics pipeline created.");
    }

    Pipeline::~Pipeline()
    {
        DX_LOG(Verbose, "Pipeline", "Graphics pipeline destroyed.");
    }

    bool Pipeline::CreateInputLayout()
    {
        if (!m_desc.m_shaders[ShaderType_Vertex])
        {
            DX_LOG(Error, "Pipeline", "Vertex shader is not specified in pipeline description.");
            return false;
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc(m_desc.m_inputLayout.m_inputElements.size());
        std::ranges::transform(m_desc.m_inputLayout.m_inputElements, inputLayoutDesc.begin(), 
            [](const InputElement& element)
            {
                D3D11_INPUT_ELEMENT_DESC inputElement = {};
                inputElement.SemanticName = ToDX11InputSemanticName(element.m_semantic, element.m_semanticCustomName.c_str());
                inputElement.SemanticIndex = element.m_semanticIndex;
                inputElement.Format = ToDX11ResourceFormat(element.m_format);
                inputElement.InputSlot = element.m_inputSlot;
                inputElement.AlignedByteOffset = element.m_alignedByteOffset;
                inputElement.InputSlotClass = (element.m_instanceDataStepRate > 0)
                    ? D3D11_INPUT_PER_INSTANCE_DATA
                    : D3D11_INPUT_PER_VERTEX_DATA;
                inputElement.InstanceDataStepRate = element.m_instanceDataStepRate;
                return inputElement;
            });

        // Once an input-layout object is created for a vertex shader signature, the input-layout object can be reused
        // with any other vertex shader that has an identical input signature (semantics included). 
        auto result = m_ownerDevice->GetDX11Device()->CreateInputLayout(
            inputLayoutDesc.data(),
            inputLayoutDesc.size(),
            m_desc.m_shaders[ShaderType_Vertex]->GetShaderDesc().m_bytecode->GetData(),
            m_desc.m_shaders[ShaderType_Vertex]->GetShaderDesc().m_bytecode->GetSize(),
            m_dx11InputLayout.GetAddressOf());

        return SUCCEEDED(result);
    }

    bool Pipeline::CreateRasterizerState()
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        rasterizerDesc.FillMode = ToDX11FillMode(m_desc.m_rasterizerState.m_faceFillMode);
        rasterizerDesc.CullMode = ToDX11CullMode(m_desc.m_rasterizerState.m_faceCullMode);
        rasterizerDesc.FrontCounterClockwise = (m_desc.m_rasterizerState.m_faceFrontOrder == FaceFrontOrder::CounterClockwise);
        rasterizerDesc.DepthBias = m_desc.m_rasterizerState.m_depthBias;
        rasterizerDesc.DepthBiasClamp = m_desc.m_rasterizerState.m_depthBiasClamp;
        rasterizerDesc.SlopeScaledDepthBias = m_desc.m_rasterizerState.m_slopeScaledDepthBias;
        rasterizerDesc.DepthClipEnable = !m_desc.m_rasterizerState.m_depthClipDisabled;
        rasterizerDesc.ScissorEnable = m_desc.m_rasterizerState.m_scissorEnabled;
        rasterizerDesc.MultisampleEnable = m_desc.m_rasterizerState.m_multisampleEnabled;
        rasterizerDesc.AntialiasedLineEnable = m_desc.m_rasterizerState.m_antialiasedLineEnabled;

        auto result = m_ownerDevice->GetDX11Device()->CreateRasterizerState(
            &rasterizerDesc,
            m_dx11RasterizerState.GetAddressOf());

        return SUCCEEDED(result);
    }

    bool Pipeline::CreateBlendState()
    {
        D3D11_BLEND_DESC blendDesc = {};
        blendDesc.AlphaToCoverageEnable = m_desc.m_blendState.m_alphaToCoverageEnabled;
        blendDesc.IndependentBlendEnable = m_desc.m_blendState.m_independentBlendEnabled;
        if (blendDesc.IndependentBlendEnable)
        {
            std::ranges::transform(m_desc.m_blendState.renderTargetBlends, std::begin(blendDesc.RenderTarget),
                [](const RenderTargetBlend& renderTargetBlend)
                {
                    D3D11_RENDER_TARGET_BLEND_DESC rtbDesc = {};
                    rtbDesc.BlendEnable = renderTargetBlend.m_blendEnabled;
                    if (rtbDesc.BlendEnable)
                    {
                        rtbDesc.SrcBlend = ToDX11Blend(renderTargetBlend.m_srcBlend);
                        rtbDesc.DestBlend = ToDX11Blend(renderTargetBlend.m_destBlend);
                        rtbDesc.BlendOp = ToDX11BlendOperation(renderTargetBlend.m_blendOp);
                        rtbDesc.SrcBlendAlpha = ToDX11Blend(renderTargetBlend.m_srcBlendAlpha);
                        rtbDesc.DestBlendAlpha = ToDX11Blend(renderTargetBlend.m_destBlendAlpha);
                        rtbDesc.BlendOpAlpha = ToDX11BlendOperation(renderTargetBlend.m_blendOpAlpha);
                    }
                    rtbDesc.RenderTargetWriteMask = ToDX11ColorWriteMask(renderTargetBlend.m_colorWriteMask);
                    return rtbDesc;
                });
        }
        else
        {
            const RenderTargetBlend& renderTargetBlend = m_desc.m_blendState.renderTargetBlends[0];
            D3D11_RENDER_TARGET_BLEND_DESC& rtbDesc = blendDesc.RenderTarget[0];
            rtbDesc.BlendEnable = renderTargetBlend.m_blendEnabled;
            if (rtbDesc.BlendEnable)
            {
                rtbDesc.SrcBlend = ToDX11Blend(renderTargetBlend.m_srcBlend);
                rtbDesc.DestBlend = ToDX11Blend(renderTargetBlend.m_destBlend);
                rtbDesc.BlendOp = ToDX11BlendOperation(renderTargetBlend.m_blendOp);
                rtbDesc.SrcBlendAlpha = ToDX11Blend(renderTargetBlend.m_srcBlendAlpha);
                rtbDesc.DestBlendAlpha = ToDX11Blend(renderTargetBlend.m_destBlendAlpha);
                rtbDesc.BlendOpAlpha = ToDX11BlendOperation(renderTargetBlend.m_blendOpAlpha);
            }
            rtbDesc.RenderTargetWriteMask = ToDX11ColorWriteMask(renderTargetBlend.m_colorWriteMask);
        }

        auto result = m_ownerDevice->GetDX11Device()->CreateBlendState(
            &blendDesc,
            m_dx11BlendState.GetAddressOf());

        return SUCCEEDED(result);
    }

    bool Pipeline::CreateDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = m_desc.m_depthStencilState.m_depthEnabled;
        if (depthStencilDesc.DepthEnable)
        {
            depthStencilDesc.DepthWriteMask = m_desc.m_depthStencilState.m_depthWriteEnabled
                ? D3D11_DEPTH_WRITE_MASK_ALL
                : D3D11_DEPTH_WRITE_MASK_ZERO;
            depthStencilDesc.DepthFunc = ToDX11ComparisonFunction(m_desc.m_depthStencilState.m_depthTestFunc);
        }
        depthStencilDesc.StencilEnable = m_desc.m_depthStencilState.m_stencilEnabled;
        if (depthStencilDesc.StencilEnable)
        {
            depthStencilDesc.StencilReadMask = m_desc.m_depthStencilState.m_stencilReadMask;
            depthStencilDesc.StencilWriteMask = m_desc.m_depthStencilState.m_stencilWriteMask;
            depthStencilDesc.FrontFace = ToDX11StencipBehaviour(m_desc.m_depthStencilState.m_frontFaceStencilBehaviour);
            depthStencilDesc.BackFace = ToDX11StencipBehaviour(m_desc.m_depthStencilState.m_backFaceStencilBehaviour);
        }

        auto result = m_ownerDevice->GetDX11Device()->CreateDepthStencilState(
            &depthStencilDesc,
            m_dx11DepthStencilState.GetAddressOf());

        return SUCCEEDED(result);
    }

    bool Pipeline::CreatePipelineResourceBindings()
    {
        PipelineResourceBindingData pipelineLayout;

        uint32_t maxSrwrvSlotCount = 0;

        for (int shaderType = 0; shaderType < ShaderType_Count; ++shaderType)
        {
            const ShaderResourceLayout* shaderResourceLayout = GetShaderResourceLayout(static_cast<ShaderType>(shaderType));
            if (!shaderResourceLayout)
            {
                continue;
            }

            pipelineLayout[shaderType].m_constantBuffers.resize(shaderResourceLayout->m_constantBuffersSlotCount);
            pipelineLayout[shaderType].m_shaderResourceViews.resize(shaderResourceLayout->m_shaderResourceViewsSlotCount);
            pipelineLayout[shaderType].m_samplers.resize(shaderResourceLayout->m_samplersSlotCount);

            maxSrwrvSlotCount = std::max<uint32_t>(maxSrwrvSlotCount, shaderResourceLayout->m_shaderRWResourceViewsSlotCount);
        }

        // Shader RW Resource Views layouts is a special case because in DirectX they
        // are not split by shader types, so we're going to collect them all under
        // the Pixel Shader. If a slot collides with different shaders, verify it's the
        // same resource, otherwise through an error.
        pipelineLayout[ShaderType_Pixel].m_shaderRWResourceViews.resize(maxSrwrvSlotCount);

#ifndef NDEBUG
        // Verify that all shared SRWRV slots have the same resource.
        std::vector<ShaderResourceInfo> verifyViews(maxSrwrvSlotCount);
        for (uint32_t shaderType = 0; shaderType < ShaderType_Count; ++shaderType)
        {
            const ShaderResourceLayout* shaderResourceLayout = GetShaderResourceLayout(static_cast<ShaderType>(shaderType));
            if (!shaderResourceLayout)
            {
                continue;
            }

            for (uint32_t i = 0; i < shaderResourceLayout->m_shaderRWResourceViews.size(); ++i)
            {
                const ShaderResourceInfo& resource = shaderResourceLayout->m_shaderRWResourceViews[i];
                for (uint32_t slot = resource.m_startSlot; slot < resource.m_startSlot + resource.m_slotCount; ++slot)
                {
                    if (verifyViews[slot].m_name.empty())
                    {
                        verifyViews[slot] = resource;
                    }
                    else
                    {
                        // Resource was already assigned to slot, verify it's the same resource.
                        if (verifyViews[slot].m_name != resource.m_name ||
                            verifyViews[slot].m_bufferSubType != resource.m_bufferSubType ||
                            verifyViews[slot].m_textureType != resource.m_textureType ||
                            verifyViews[slot].m_textureSubTypeFlags != resource.m_textureSubTypeFlags)
                        {
                            DX_LOG(Fatal, "Pipeline", 
                                "Different shaders have different read-write buffers or textures assigned to same slot (%u).", slot);
                        }
                    }
                }
            }
        }
#endif

        m_resourceBindings = PipelineResourceBindings(this, std::move(pipelineLayout));
        return true;
    }

    std::shared_ptr<Shader> Pipeline::GetPipelineShader(ShaderType shaderType)
    {
        return m_desc.m_shaders[shaderType];
    }

    std::shared_ptr<const Shader> Pipeline::GetPipelineShader(ShaderType shaderType) const
    {
        return m_desc.m_shaders[shaderType];
    }

    const ShaderResourceLayout* Pipeline::GetShaderResourceLayout(ShaderType shaderType) const
    {
        auto shader = m_desc.m_shaders[shaderType];
        if (!shader)
        {
            return nullptr;
        }
        DX_ASSERT(shader->GetShaderDesc().m_bytecode.get(), "Pipeline",
            "%s Shader Bytecode is null.", ShaderTypeStr(shaderType));

        return &shader->GetShaderDesc().m_bytecode->GetShaderResourceLayout();
    }

    ComPtr<ID3D11InputLayout> Pipeline::GetDX11InputLayout()
    {
        return m_dx11InputLayout;
    }

    ComPtr<ID3D11RasterizerState> Pipeline::GetDX11RasterizerState()
    {
        return m_dx11RasterizerState;
    }

    ComPtr<ID3D11BlendState> Pipeline::GetDX11BlendState()
    {
        return m_dx11BlendState;
    }

    ComPtr<ID3D11DepthStencilState> Pipeline::GetDX11DepthStencilState()
    {
        return m_dx11DepthStencilState;
    }
} // namespace DX
