#pragma once

namespace DX
{
    enum class DeviceObjectType
    {
        Invalid = 0,

        DeviceContext,
        SwapChain,
        FrameBuffer,
        Buffer,
        Texture,
        RenderTargetView,
        DepthStencilView,
        Sampler,
        Shader,
        ShaderResourceView,
        ShaderRWResourceView,
        Pipeline,
        ResourceLayout,
        CommandList,

        Count
    };
} // namespace DX
