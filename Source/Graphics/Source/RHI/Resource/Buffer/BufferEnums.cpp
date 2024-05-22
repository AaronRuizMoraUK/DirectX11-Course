#include <RHI/Resource/Buffer/BufferEnums.h>

namespace DX
{
    const char* BufferTypeStr(BufferType bufferType)
    {
        switch (bufferType)
        {
        case BufferType::None:
            return "";
        case BufferType::Typed:
            return "Typed";
        case BufferType::Structured:
            return "Structured";
        case BufferType::Raw:
            return "Raw";
        default:
            return "Unknown";
        }
    }
} // namespace DX
