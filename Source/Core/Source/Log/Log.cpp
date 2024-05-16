#include <Log/Log.h>

#include <cstdio>
#include <cstdarg>
#include <cassert>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace DX::Internal
{
    namespace
    {
        enum class LogColor
        {
            Normal,
            Yellow,
            Red,
            Blue
        };

        void DebugOutput(LogColor logColor, const char* message)
        {
            auto colorStr = [](LogColor logColor)
                {
                    constexpr const char* LogNormal = "\033[39m";
                    constexpr const char* LogYellow = "\033[33m";
                    constexpr const char* LogRed = "\033[31m";
                    constexpr const char* LogBlue = "\033[34m";

                    switch (logColor)
                    {
                    case LogColor::Normal: return LogNormal;
                    case LogColor::Yellow: return LogYellow;
                    case LogColor::Red: return LogRed;
                    case LogColor::Blue: return LogBlue;
                    default: return LogNormal;
                    }
                };

            std::printf("%s%s%s", colorStr(logColor), message, colorStr(LogColor::Normal));

#ifdef _WIN32
            OutputDebugString(message);
#endif
        }
    }

    void Log(LogLevel level, const char* title, const char* message, ...)
    {
        char buffer[8 * 1024];
        char* head = buffer;

        LogColor logColor = LogColor::Normal;
        head += std::sprintf(head, "[%s] ", title);

        switch(level)
        {
            case LogLevel::Verbose:
                logColor = LogColor::Blue;
                break;
            case LogLevel::Warning:
                logColor = LogColor::Yellow;
                head += std::sprintf(head, "Warning: ");
                break;
            case LogLevel::Error:
                logColor = LogColor::Red;
                head += std::sprintf(head, "Error: ");
                break;
            case LogLevel::Fatal:
                logColor = LogColor::Red;
                head += std::sprintf(head, "Fatal Error: ");
                break;
            case LogLevel::Info:
            default:
                break;
        }

        va_list args;
        va_start(args, message);
        head += vsprintf(head, message, args);
        va_end(args);

        std::sprintf(head, "\n");

        DebugOutput(logColor, buffer);

        if (level == LogLevel::Fatal)
        {
            abort();
        }
    }

    void Assert(bool condition, const char* conditionStr, const char* title, const char* file, int line, const char* message, ...)
    {
        if (!condition)
        {
            char buffer[8 * 1024];
            char* head = buffer;

            head += std::sprintf(head, "[%s] Assertion failed: ", title);

            va_list args;
            va_start(args, message);
            head += vsprintf(head, message, args);
            va_end(args);

            std::sprintf(head, "\nFile: %s, line %d\nCondition: %s\n", file, line, conditionStr);

            DebugOutput(LogColor::Red, buffer);

            assert(condition);
        }
    }
} // namespace DX::Internal
