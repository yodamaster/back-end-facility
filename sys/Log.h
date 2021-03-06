#ifndef __LOG_LAYER_H__
#define __LOG_LAYER_H__

// switch to toggle log module
#define LOG_ENABLE (1)

#if LOG_ENABLE

#include <stdarg.h>

// todo
// provide signal event handler.
// flush log when program crashes.
enum LogLevel
{
    LOG_ALL = 0,
    LOG_VERB, // verbose
    LOG_INFO,
    LOG_DEBUG,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,

    LOG_LEVELS
};

void slog_all(const char* format, va_list va);
void slog_debug(const char* format, va_list va);
void slog_info(const char* format, va_list va);
void slog_verb(const char* format, va_list va);
void slog_warn(const char* format, va_list va);
void slog_error(const char* format, va_list va);
void slog_fatal(const char* format, va_list va);

int  slog_level();
void InitLogger();

// not thread safe
void SetLogLevel(int level);

inline void slog(LogLevel level, const char* format, ...)
{
    int threshold = slog_level();

    if (level < 0 || level < threshold) return;

    typedef void (* proc)(const char*, va_list);
    static const proc arr[LOG_LEVELS] =
    {
        slog_all,
        slog_verb,
        slog_info,
        slog_debug,
        slog_warn,
        slog_error,
        slog_fatal
    };

    va_list args;
    va_start(args, format);

    (arr[level])(format, args);

    va_end(args);
}

#else
    #define slog(s,...)
#endif

#endif // __LOG_LAYER_H__

