#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Pipeline/PipelineDesc.h>

#include <RHI/DirectX/ComPtr.h>
class ID3D11InputLayout;
class ID3D11RasterizerState;
class ID3D11BlendState;
class ID3D11DepthStencilState;

namespace DX
{
    class Pipeline : public DeviceObject
    {
    public:
        Pipeline(Device* device, const PipelineDesc& desc);
        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        DeviceObjectType GetType() const override { return DeviceObjectType::Pipeline; }

        const PipelineDesc& GetPipelineDesc() const { return m_desc; }

        std::shared_ptr<Shader> GetPipelineShader(ShaderType shaderType);

        ComPtr<ID3D11InputLayout> GetDX11InputLayout();
        ComPtr<ID3D11RasterizerState> GetDX11RasterizerState();
        ComPtr<ID3D11BlendState> GetDX11BlendState();
        ComPtr<ID3D11DepthStencilState> GetDX11DepthStencilState();

    private:
        PipelineDesc m_desc;

    private:
        bool CreateInputLayout();
        bool CreateRasterizerState();
        bool CreateBlendState();
        bool CreateDepthStencilState();

        ComPtr<ID3D11InputLayout> m_dx11InputLayout;
        ComPtr<ID3D11RasterizerState> m_dx11RasterizerState;
        ComPtr<ID3D11BlendState> m_dx11BlendState;
        ComPtr<ID3D11DepthStencilState> m_dx11DepthStencilState;
    };
} // namespace DX