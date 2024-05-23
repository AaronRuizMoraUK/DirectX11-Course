#pragma once

// -------------------------------------------------------
// Usage:
//
// DX_ASSERT(condition, "Title", "My message %d", 123);
// 
// #pragma DX_IGNORE_WARNING(4101, "-Wunused-variable")
// -------------------------------------------------------

#ifndef NDEBUG
// No Release configuration

#define DX_ASSERT(condition, title, message, ...)   ::DX::Internal::Assert((condition), #condition, title, __FILE__, __LINE__, message, ##__VA_ARGS__)

#else
// Release configuration

#define DX_ASSERT(...)

#endif // NDEBUG


#if defined(__GNUC__)

#define DX_DISABLE_WARNING(msvc_warning_number, gcc_clang_warning_string)    GCC diagnostic ignored gcc_clang_warning_string

#elif defined(_MSC_VER)

#define DX_DISABLE_WARNING(msvc_warning_number, gcc_clang_warning_string)    warning(disable : msvc_warning_number)

#else

#define DX_DISABLE_WARNING(msvc_warning_number, gcc_clang_warning_string)

#endif

namespace DX::Internal
{
    void Assert(bool condition, const char* conditionStr, const char* title, const char* file, int line, const char* message, ...);

    enum class LogColor
    {
        Normal,
        Yellow,
        Red,
        Blue
    };

    void DebugOutput(LogColor logColor, const char* message);
} // namespace DX::Internal
