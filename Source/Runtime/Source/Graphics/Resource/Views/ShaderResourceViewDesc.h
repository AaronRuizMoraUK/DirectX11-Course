#pragma once

#include <variant>
#include <memory>

namespace DX
{
    class Texture;
    class Buffer;

    // This view only supports texture and buffer resources.
    struct ShaderResourceViewDesc
    {
        using Resource = std::variant<std::shared_ptr<Texture>, std::shared_ptr<Buffer>>;
        Resource m_resource;
    };
} // namespace DX
