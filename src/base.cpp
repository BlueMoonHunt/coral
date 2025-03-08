#include "base.hpp"

const char* getLogLevelName(LogLevel level) {
    switch (level) {
    case LogLevel::INFO: return "Info";
    case LogLevel::TRACE: return "Trace";
    case LogLevel::WARNING: return "Warning";
    case LogLevel::ERROR: return "Error";
    case LogLevel::FETAL: return "Fetal";
    }
    return "Unknown";
}