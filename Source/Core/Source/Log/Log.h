#pragma once

// -------------------------------------------------------
// Usage:
//
// DX_LOG(Info, "Title", "My message %d", 123);
// DX_LOG(Warning, "Title", "My message %d", 123);
// DX_LOG(Error, "Title", "My message %d", 123);
//
// DX_ASSERT(condition, "Title", "My message %d", 123);
// -------------------------------------------------------

#ifndef NDEBUG
// No Release configuration

#define DX_LOG(level, title, message, ...)          ::DX::Internal::Log(::DX::Internal::LogLevel::level, title, message, ##__VA_ARGS__)
#define DX_ASSERT(condition, title, message, ...)   ::DX::Internal::Assert((condition), #condition, title, __FILE__, __LINE__, message, ##__VA_ARGS__)

#else
// Release configuration

#define DX_ASSERT(...)
#define DX_LOG(...)

#endif // NDEBUG

namespace DX::Internal
{
    enum class LogLevel
    {
        Info,
        Warning,
        Error
    };

    void Log(LogLevel level, const char* title, const char* message, ...);

    void Assert(bool condition, const char* conditionStr, const char* title, const char* file, int line, const char* message, ...);
} // namespace DX::Internal
