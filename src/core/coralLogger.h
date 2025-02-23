#pragma once
#include <string>

namespace coral::Log {
    enum LogLevel {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warn = 3,
        Error = 4,
        Critical = 5,
    };

    void log(const std::string& message, LogLevel level);
    void printLogs();

} // namespace coral::Log