#include <Log/Log.h>

#include <cstdio>
#include <cstdarg>
#include <cassert>

namespace DX::Internal
{
    namespace
    {
        constexpr const char* LogNormal = "\033[39m";
        constexpr const char* LogYellow = "\033[33m";
        constexpr const char* LogRed = "\033[31m";
    }

    void Log(LogLevel level, const char* title, const char* message, ...)
    {
        switch(level)
        {
            case LogLevel::Info:
                std::printf("[%s] ", title);
                break;
            case LogLevel::Warning:
                std::printf("%s[%s] Warning: ", LogYellow, title);
                break;
            case LogLevel::Error:
                std::printf("%s[%s] Error: ", LogRed, title);
                break;
            default:
                break;
        }

        va_list args;
        va_start(args, message);
        vprintf(message, args);
        va_end(args);

        std::printf("%s\n", LogNormal);
    }

    void Assert(bool condition, const char* title, const char* message, ...)
    {
        if (!condition)
        {
            std::printf("%s[%s] Assert: ", LogRed, title);

            va_list args;
            va_start(args, message);
            vprintf(message, args);
            va_end(args);

            std::printf("%s\n", LogNormal);

            assert(condition);
        }
    }
} // namespace DX::Internal
