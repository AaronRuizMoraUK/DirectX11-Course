#include <Graphics/Pipeline/Pipeline.h>

#include <Graphics/Device/Device.h>
#include <Graphics/Shader/ShaderBytecode.h>
#include <Graphics/Pipeline/InputLayout/InputLayout.h>
#include <Graphics/Pipeline/RasterizerState/RasterizerState.h>
#include <Graphics/Pipeline/BlendState/BlendState.h>
#include <Log/Log.h>

#include <ranges>

#include <d3d11.h>
#include <Graphics/DirectX/Utils.h>

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

        DX_LOG(Info, "Pipeline", "Graphics pipeline created.");
    }

    Pipeline::~Pipeline()
    {
        DX_LOG(Info, "Pipeline", "Graphics pipeline destroyed.");
    }

    bool Pipeline::CreateInputLayout()
    {
        if (!m_desc.m_shaders[ShaderType_Vertex])
        {
            DX_LOG(Error, "Pipeline", "Vertex shader is not specified in pipeline description.");
            return false;
        }

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc(m_desc.m_inputLayout.size());
        std::ranges::transform(m_desc.m_inputLayout, inputLayoutDesc.begin(), [](const InputElement& element)
            {
                D3D11_INPUT_ELEMENT_DESC inputElement = {};
                inputElement.SemanticName = element.m_semanticName.c_str();
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
        std::ranges::transform(m_desc.m_blendState.renderTargetBlends, std::begin(blendDesc.RenderTarget),
            [](const RenderTargetBlend& renderTargetBlend)
            {
                D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
                renderTargetBlendDesc.BlendEnable = renderTargetBlend.m_blendEnabled;
                renderTargetBlendDesc.SrcBlend = ToDX11Blend(renderTargetBlend.m_srcBlend);
                renderTargetBlendDesc.DestBlend = ToDX11Blend(renderTargetBlend.m_destBlend);
                renderTargetBlendDesc.BlendOp = ToDX11BlendOperation(renderTargetBlend.m_blendOp);
                renderTargetBlendDesc.SrcBlendAlpha = ToDX11Blend(renderTargetBlend.m_srcBlendAlpha);
                renderTargetBlendDesc.DestBlendAlpha = ToDX11Blend(renderTargetBlend.m_destBlendAlpha);
                renderTargetBlendDesc.BlendOpAlpha = ToDX11BlendOperation(renderTargetBlend.m_blendOpAlpha);
                renderTargetBlendDesc.RenderTargetWriteMask = renderTargetBlend.m_renderTargetWriteMask;
                return renderTargetBlendDesc;
            });

        auto result = m_ownerDevice->GetDX11Device()->CreateBlendState(
            &blendDesc,
            m_dx11BlendState.GetAddressOf());

        return SUCCEEDED(result);
    }

    bool Pipeline::CreateDepthStencilState()
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        //OOL DepthEnable;
        //3D11_DEPTH_WRITE_MASK DepthWriteMask;
        //3D11_COMPARISON_FUNC DepthFunc;
        //OOL StencilEnable;
        //INT8 StencilReadMask;
        //INT8 StencilWriteMask;
        //3D11_DEPTH_STENCILOP_DESC FrontFace;
        //3D11_DEPTH_STENCILOP_DESC BackFace;

        auto result = m_ownerDevice->GetDX11Device()->CreateDepthStencilState(
            &depthStencilDesc,
            m_dx11DepthStencilState.GetAddressOf());

        return SUCCEEDED(result);
    }
} // namespace DX
