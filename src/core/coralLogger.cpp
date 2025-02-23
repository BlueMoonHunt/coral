#include "coralLogger.h"

#include "core/application.h"

#include <queue>
#include <print>

namespace coral::Log {

    struct CoralLog {
        std::string message = "";
        LogLevel level = Trace;
    };

    static std::queue<CoralLog> s_Logs;
    constexpr uint32_t logLimit = 200;

    void log(const std::string& message, LogLevel level) {
        if (!(Application::Get().getApplicationFlags() & ApplicationFlag_Debug))
            return;

        if (s_Logs.size() > logLimit)
            s_Logs.pop();

        switch (level) {
        case LogLevel::Trace:       s_Logs.emplace(CoralLog{ message, level });  break;
        case LogLevel::Debug:       s_Logs.emplace(CoralLog{ message, level });  break;
        case LogLevel::Info:        s_Logs.emplace(CoralLog{ message, level });  break;
        case LogLevel::Warn:        s_Logs.emplace(CoralLog{ message, level });  break;
        case LogLevel::Error:       s_Logs.emplace(CoralLog{ message, level });  break;
        case LogLevel::Critical:    s_Logs.emplace(CoralLog{ message, level });  break;
        }
    }

    // ANSI escape codes for colors
    const char* getColor(LogLevel level) {
        switch (level) {
        case LogLevel::Trace:    return "\033[37m"; // White
        case LogLevel::Debug:    return "\033[36m"; // Cyan
        case LogLevel::Info:     return "\033[32m"; // Green
        case LogLevel::Warn:     return "\033[33m"; // Yellow
        case LogLevel::Error:    return "\033[31m"; // Red
        case LogLevel::Critical: return "\033[35m"; // Magenta
        default:                 return "\033[0m";  // Reset
        }
    }

    void printLogs() {
        while (!s_Logs.empty()) {
            std::println("{}{}\033[0m", getColor(s_Logs.front().level), s_Logs.front().message);
            s_Logs.pop();
        }
    }

}