#include <RHI/Resource/Buffer/BufferEnums.h>

namespace DX
{
    const char* BufferSubTypeStr(BufferSubType bufferSubType)
    {
        switch (bufferSubType)
        {
        case BufferSubType::None:
            return "";
        case BufferSubType::Typed:
            return "Typed";
        case BufferSubType::Structured:
            return "Structured";
        case BufferSubType::Raw:
            return "Raw";
        default:
            return "Unknown";
        }
    }
} // namespace DX
