#include <Graphics/Pipeline/Pipeline.h>

#include <Graphics/Device/Device.h>
#include <Log/Log.h>

#include <d3d11.h>

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
        return false;
    }

    bool Pipeline::CreateRasterizerState()
    {
        //D3D11_RASTERIZER_DESC rasterizerDesc;
        //D3D11_FILL_MODE FillMode;
        //D3D11_CULL_MODE CullMode;
        //BOOL FrontCounterClockwise;
        //INT DepthBias;
        //FLOAT DepthBiasClamp;
        //FLOAT SlopeScaledDepthBias;
        //BOOL DepthClipEnable;
        //BOOL ScissorEnable;
        //BOOL MultisampleEnable;
        //BOOL AntialiasedLineEnable;
        //
        //ComPtr<ID3D11RasterizerState> m_dx11RasterizerState;
        return false;
    }

    bool Pipeline::CreateBlendState()
    {
        //D3D11_BLEND_DESC blendDesc;
        //BOOL AlphaToCoverageEnable;
        //BOOL IndependentBlendEnable;
        //D3D11_RENDER_TARGET_BLEND_DESC RenderTarget[8];
        //
        //ComPtr<ID3D11BlendState> m_dx11BlendState;
        return false;
    }

    bool Pipeline::CreateDepthStencilState()
    {
        //3D11_DEPTH_STENCIL_DESC depthStencilDesc;
        //OOL DepthEnable;
        //3D11_DEPTH_WRITE_MASK DepthWriteMask;
        //3D11_COMPARISON_FUNC DepthFunc;
        //OOL StencilEnable;
        //INT8 StencilReadMask;
        //INT8 StencilWriteMask;
        //3D11_DEPTH_STENCILOP_DESC FrontFace;
        //3D11_DEPTH_STENCILOP_DESC BackFace;
        //
        //omPtr<ID3D11DepthStencilState> m_dx11DepthStencilState;
        return false;
    }
} // namespace DX
