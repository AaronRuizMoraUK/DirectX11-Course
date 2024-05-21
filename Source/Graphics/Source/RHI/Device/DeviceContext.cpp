#include <RHI/Device/DeviceContext.h>

#include <RHI/Device/Device.h>
#include <RHI/FrameBuffer/FrameBuffer.h>
#include <RHI/Pipeline/Pipeline.h>
#include <RHI/Resource/Buffer/Buffer.h>
#include <RHI/Resource/Views/RenderTargetView.h>
#include <RHI/Resource/Views/DepthStencilView.h>
#include <RHI/Resource/Views/ShaderResourceView.h>
#include <RHI/Resource/Views/ShaderRWResourceView.h>
#include <RHI/Sampler/Sampler.h>
#include <Log/Log.h>

#include <d3d11.h>
#include <RHI/DirectX/Utils.h>

namespace DX
{
    DeviceContext::DeviceContext(Device* device, DeviceContextType type, void* nativeContext)
        : DeviceObject(device)
        , m_type(type)
    {
        switch (m_type)
        {
        case DeviceContextType::Immediate:
            // Immediate context must be passed by parameter.
            if (nativeContext)
            {
                m_dx11DeviceContext = static_cast<ID3D11DeviceContext*>(nativeContext);
            }
            else
            {
                DX_LOG(Fatal, "DeviceContext", "Invalid immediate context device.");
                return;
            }
            DX_LOG(Verbose, "DeviceContext", "Graphics immediate device context created.");
            break;

        case DeviceContextType::Deferred:
            if (nativeContext)
            {
                m_dx11DeviceContext = static_cast<ID3D11DeviceContext*>(nativeContext);
            }
            else
            {
                // If deferred const is not passed by parameter, create it.
                auto result = m_ownerDevice->GetDX11Device()->CreateDeferredContext(0, m_dx11DeviceContext.GetAddressOf());
                if (FAILED(result))
                {
                    DX_LOG(Fatal, "DeviceContext", "Failed to create deferred device context.");
                    return;
                }
            }
            DX_LOG(Verbose, "DeviceContext", "Graphics deferred device context created.");
            break;

        case DeviceContextType::Unknown:
        default:
            DX_LOG(Fatal, "DeviceContext", "Unknown device context type.");
            return;
        }
    }

    DeviceContext::~DeviceContext()
    {
        if (m_dx11DeviceContext)
        {
            DX_LOG(Verbose, "DeviceContext", "Graphics device context destroyed.");
        }
    }

    void DeviceContext::BindFrameBuffer(FrameBuffer& frameBuffer)
    {
        m_dx11DeviceContext->OMSetRenderTargets(1,
            frameBuffer.GetColorRenderTargetView() 
                ? frameBuffer.GetColorRenderTargetView()->GetDX11RenderTargetView().GetAddressOf() 
                : nullptr,
            frameBuffer.GetDepthStencilView() 
                ? frameBuffer.GetDepthStencilView()->GetDX11DepthStencilView().Get() 
                : nullptr);
    }

    void DeviceContext::BindPipeline(Pipeline& pipeline)
    {
        for (int i = 0; i< ShaderType_Count; ++i)
        {
            std::shared_ptr<Shader> shader = pipeline.GetPipelineShader(static_cast<ShaderType>(i));
            switch (i)
            {
            case ShaderType_Unknown:
                break;

            case ShaderType_Vertex:
                m_dx11DeviceContext->VSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11VertexShader>() : nullptr, nullptr, 0);
                break;

            case ShaderType_Hull:
                m_dx11DeviceContext->HSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11HullShader>() : nullptr, nullptr, 0);
                break;

            case ShaderType_Domain:
                m_dx11DeviceContext->DSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11DomainShader>() : nullptr, nullptr, 0);
                break;

            case ShaderType_Geometry:
                m_dx11DeviceContext->GSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11GeometryShader>() : nullptr, nullptr, 0);
                break;

            case ShaderType_Pixel:
                m_dx11DeviceContext->PSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11PixelShader>() : nullptr, nullptr, 0);
                break;

            case ShaderType_Compute:
                m_dx11DeviceContext->CSSetShader(shader ? shader->GetDX11ShaderAs<ID3D11ComputeShader>() : nullptr, nullptr, 0);
                break;

            default:
                DX_LOG(Fatal, "DeviceContext", "Unknown shader type %d", shader->GetShaderType());
                break;
            }
        }

        m_dx11DeviceContext->IASetInputLayout(pipeline.GetDX11InputLayout().Get());
        m_dx11DeviceContext->IASetPrimitiveTopology(ToDX11PrimitiveTopology(
            pipeline.GetPipelineDesc().m_inputLayout.m_primitiveTopology,
            pipeline.GetPipelineDesc().m_inputLayout.m_controlPointPatchListCount));

        m_dx11DeviceContext->RSSetState(pipeline.GetDX11RasterizerState().Get());
        m_dx11DeviceContext->OMSetBlendState(pipeline.GetDX11BlendState().Get(), nullptr, 0xFFFFFFFF);
        m_dx11DeviceContext->OMSetDepthStencilState(pipeline.GetDX11DepthStencilState().Get(), 0);
    }

    void DeviceContext::BindViewports(const std::vector<Math::Rectangle>& rectangles)
    {
        std::vector<D3D11_VIEWPORT> viewports(rectangles.size());
        std::ranges::transform(rectangles, viewports.begin(), 
            [](const Math::Rectangle& rectangle)
            {
                D3D11_VIEWPORT viewport = {};
                viewport.TopLeftX = rectangle.pos.x;
                viewport.TopLeftY = rectangle.pos.y;
                viewport.Width = rectangle.size.x;
                viewport.Height = rectangle.size.y;
                viewport.MinDepth = 0.0f;
                viewport.MaxDepth = 1.0f;
                return viewport;
            });

        m_dx11DeviceContext->RSSetViewports(viewports.size(), viewports.data());
    }

    void DeviceContext::BindScissors(const std::vector<Math::RectangleInt>& rectangles)
    {
        std::vector<D3D11_RECT> scissorRects(rectangles.size());
        std::ranges::transform(rectangles, scissorRects.begin(),
            [](const Math::RectangleInt& rectangle)
            {
                D3D11_RECT rect = {};
                rect.left = rectangle.pos.x;
                rect.top = rectangle.pos.y;
                rect.right = rectangle.pos.x + rectangle.size.x;
                rect.bottom = rectangle.pos.y + rectangle.size.y;
                return rect;
            });

        m_dx11DeviceContext->RSSetScissorRects(scissorRects.size(), scissorRects.data());
    }

    void DeviceContext::BindVertexBuffers(const std::vector<Buffer*>& vertexBuffers)
    {
        for (int i = 0; i < vertexBuffers.size(); ++i)
        {
            const uint32_t vertexBufferStride = vertexBuffers[i]->GetBufferDesc().m_elementSizeInBytes;
            const uint32_t vertexBufferOffset = 0;

            m_dx11DeviceContext->IASetVertexBuffers(
                i, 1, vertexBuffers[i]->GetDX11Buffer().GetAddressOf(), &vertexBufferStride, &vertexBufferOffset);
        }
    }

    void DeviceContext::BindIndexBuffer(Buffer& indexBuffer)
    {
        ResourceFormat indexFormat;
        switch (indexBuffer.GetBufferDesc().m_elementSizeInBytes)
        {
        case 2:
            indexFormat = ResourceFormat::R16_UINT;
            break;
        case 4:
            indexFormat = ResourceFormat::R32_UINT;
            break;
        default:
            DX_LOG(Fatal, "DeviceContext", "Index buffer format not supported.");
            return;
        }

        m_dx11DeviceContext->IASetIndexBuffer(indexBuffer.GetDX11Buffer().Get(), ToDX11ResourceFormat(indexFormat), 0);
    }

    void DeviceContext::BindResources(uint32_t slot, std::vector<DeviceObject*> resources)
    {
        // TODO: Bind resources to shaders using resource layout.

        if (resources.empty())
        {
            DX_LOG(Warning, "DeviceContext", "BindResources called with no resources.");
            return;
        }

        // All resources passed must be of the same type.
        const DeviceObjectType type = resources[0]->GetType();

#ifndef NDEBUG
        // All resources must be of the same type.
        bool sameType = std::all_of(resources.cbegin() + 1, resources.cend(),
            [type](const DeviceObject* resource)
            {
                return resource->GetType() == type;
            });
        DX_ASSERT(sameType, "DeviceContext", "Not all resources are of the same type (%d).", type);
#endif

        switch (type)
        {
        case DeviceObjectType::Buffer:
        {
#ifndef NDEBUG
            bool allSame = std::all_of(resources.cbegin() + 1, resources.cend(),
                [type](const DeviceObject* resource)
                {
                    auto buffer = static_cast<const Buffer*>(resource);
                    return buffer->GetBufferDesc().m_bindFlags & BufferBind_ConstantBuffer;
                });
            DX_ASSERT(allSame, "DeviceContext", "Not all resources passed are constant buffers.");
#endif

            std::vector<ID3D11Buffer*> dx11Buffers(resources.size());
            std::transform(resources.begin(), resources.end(), dx11Buffers.begin(),
                [](DeviceObject* resource)
                {
                    return static_cast<Buffer*>(resource)->GetDX11Buffer().Get();
                });

            m_dx11DeviceContext->VSSetConstantBuffers(slot, resources.size(), dx11Buffers.data());
        }
        break;

        case DeviceObjectType::ShaderResourceView:
        {
            std::vector<ID3D11ShaderResourceView*> dx11SRVs(resources.size());
            std::transform(resources.begin(), resources.end(), dx11SRVs.begin(),
                [](DeviceObject* resource)
                {
                    return static_cast<ShaderResourceView*>(resource)->GetDX11ShaderResourceView().Get();
                });

            m_dx11DeviceContext->PSSetShaderResources(slot, dx11SRVs.size(), dx11SRVs.data());
        }
        break;

        case DeviceObjectType::ShaderRWResourceView:
        {
            std::vector<ID3D11UnorderedAccessView*> dx11UAVs(resources.size());
            std::transform(resources.begin(), resources.end(), dx11UAVs.begin(),
                [](DeviceObject* resource)
                {
                    return static_cast<ShaderRWResourceView*>(resource)->GetDX11UnorderedAccessView().Get();
                });

            // An array of append and consume buffer offsets. A value of - 1 indicates to keep the current offset.
            // Any other values set the hidden counter for that appendable and consumable UAV.
            // This is relevant only for UAVs that were created with either D3D11_BUFFER_UAV_FLAG_APPEND or
            // D3D11_BUFFER_UAV_FLAG_COUNTER specified when the UAV was created; otherwise, the argument is ignored.
            const UINT uavInitialCounts = -1;

            m_dx11DeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(
                D3D11_KEEP_RENDER_TARGETS_AND_DEPTH_STENCIL,
                nullptr,
                nullptr,
                slot,
                dx11UAVs.size(),
                dx11UAVs.data(),
                &uavInitialCounts);
        }
        break;

        case DeviceObjectType::Sampler:
        {
            std::vector<ID3D11SamplerState*> dx11Samplers(resources.size());
            std::transform(resources.begin(), resources.end(), dx11Samplers.begin(),
                [](DeviceObject* resource)
                {
                    return static_cast<Sampler*>(resource)->GetDX11Sampler().Get();
                });

            m_dx11DeviceContext->PSSetSamplers(slot, dx11Samplers.size(), dx11Samplers.data());
        }
        break;

        default:
            DX_LOG(Error, "DeviceContext", "Unexpected device object type %d to bind.", type);
            return;
        }
    }

    void DeviceContext::ClearFrameBuffer(FrameBuffer& frameBuffer,
        std::optional<Math::Color> color,
        std::optional<float> depth,
        std::optional<uint8_t> stencil)
    {
        if (auto rtv = frameBuffer.GetColorRenderTargetView();
            rtv && color.has_value())
        {
            m_dx11DeviceContext->ClearRenderTargetView(
                rtv->GetDX11RenderTargetView().Get(), Math::ColorPacked(*color).data_);
        }

        if (auto dsv = frameBuffer.GetDepthStencilView())
        {
            if (depth.has_value() || stencil.has_value())
            {
                uint32_t flags = (depth.has_value() ? D3D11_CLEAR_DEPTH : 0) | (stencil.has_value() ? D3D11_CLEAR_STENCIL : 0);

                m_dx11DeviceContext->ClearDepthStencilView(
                    dsv->GetDX11DepthStencilView().Get(), flags, depth.value_or(1.0f), stencil.value_or(0));
            }
        }
    }

    void DeviceContext::DrawIndexed(uint32_t indexCount, uint32_t indexOffset, uint32_t vertexOffset)
    {
        m_dx11DeviceContext->DrawIndexed(indexCount, indexOffset, vertexOffset);
    }

    void DeviceContext::UpdateDynamicBuffer(Buffer& buffer, const void* data, uint32_t dataSize)
    {
        D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
        m_dx11DeviceContext->Map(buffer.GetDX11Buffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
        memcpy(mappedSubresource.pData, data, dataSize);
        m_dx11DeviceContext->Unmap(buffer.GetDX11Buffer().Get(), 0);
    }

    ComPtr<ID3D11DeviceContext> DeviceContext::GetDX11DeviceContext()
    {
        return m_dx11DeviceContext;
    }
} // namespace DX
