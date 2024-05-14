#pragma once

namespace DX
{
    enum class DeviceObjectType
    {
        Invalid = 0,

        SwapChain,
        FrameBuffer,
        Buffer,
        Texture,
        Sampler,
        Shader,
        Pipeline,
        ResourceLayout,
        CommandList,

        Num
    };
} // namespace DX
