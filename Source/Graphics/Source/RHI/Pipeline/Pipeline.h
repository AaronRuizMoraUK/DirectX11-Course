#pragma once

#include <RHI/DeviceObject/DeviceObject.h>
#include <RHI/Pipeline/PipelineDesc.h>
#include <RHI/Pipeline/PipelineResourceBindings.h>

#include <RHI/DirectX/ComPtr.h>
struct ID3D11InputLayout;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;

namespace DX
{
    class Device;
    class Shader;
    struct ShaderResourceLayout;

    // Pipeline is the configuration of all graphics stages:
    // - Shaders used in each stage
    // - Input layout
    // - Rasterizer state
    // - Blend state
    // - Depth stencil state
    //
    // It also creates the resource bindings object, that can be used to bind resources
    // to the pipeline using Device Context.
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
        std::shared_ptr<const Shader> GetPipelineShader(ShaderType shaderType) const;
        const ShaderResourceLayout* GetShaderResourceLayout(ShaderType shaderType) const;

        ComPtr<ID3D11InputLayout> GetDX11InputLayout();
        ComPtr<ID3D11RasterizerState> GetDX11RasterizerState();
        ComPtr<ID3D11BlendState> GetDX11BlendState();
        ComPtr<ID3D11DepthStencilState> GetDX11DepthStencilState();

        // The object returned has the layout necessary from the shaders of this pipeline.
        // It'll have the right number of slots, but the slots will be empty and have to
        // be filled before using it in DeviceContext::BindResources. The pipeline is not
        // responsible for filling the resources.
        std::shared_ptr<PipelineResourceBindings> CreateResourceBindingsObject() const
        {
            return std::make_shared<PipelineResourceBindings>(m_resourceBindings);
        }

    private:
        PipelineDesc m_desc;

    private:
        bool CreateInputLayout();
        bool CreateRasterizerState();
        bool CreateBlendState();
        bool CreateDepthStencilState();
        bool CreatePipelineResourceBindings();

        ComPtr<ID3D11InputLayout> m_dx11InputLayout;
        ComPtr<ID3D11RasterizerState> m_dx11RasterizerState;
        ComPtr<ID3D11BlendState> m_dx11BlendState;
        ComPtr<ID3D11DepthStencilState> m_dx11DepthStencilState;

        PipelineResourceBindings m_resourceBindings;
    };
} // namespace DX
