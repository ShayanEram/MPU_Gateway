#ifndef INC_LOGGER_HPP
#define INC_LOGGER_HPP

#include <cstdarg>
#include <string>

enum class LogLevel { Debug, Info, Warn, Error, Fatal };

class Logger {
public:
    static void Init(LogLevel level = LogLevel::Info);
    static void SetLevel(LogLevel level);
    static LogLevel GetLevel();

    static void Log(LogLevel level, const char* fmt, ...);
    static void Debug(const char* fmt, ...);
    static void Info(const char* fmt, ...);
    static void Warn(const char* fmt, ...);
    static void Error(const char* fmt, ...);

private:
    Logger() = delete;
};

#endif // INC_LOGGER_HPP