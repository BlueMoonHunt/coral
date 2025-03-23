#pragma once
#include <memory>
#include <array>
#include <string>
#include <print>

#include "util/math.hpp"


template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> CreateRef(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using Scope = std::unique_ptr<T>;


template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}


//////////////////

enum class LogLevel {
    INFO = 0,
    TRACE,
    WARNING,
    ERROR,
    FETAL,
};

const char* getLogLevelName(LogLevel level);

#ifdef NDEBUG
#define BMH_ASSERT(expr, message)
#define BMH_LOG(expr, logLevel)
#define BMH_INFO(expr)
#define BMH_TRACE(expr)
#define BMH_WARNING(expr)
#define BMH_ERROR(expr)
#else
#include <iostream>
#include <cstdlib>
#include <print>

#define BMH_ASSERT(expr, message) {\
    if(!(expr)) {\
        std::println(std::cerr, "{}: {} at {}:{}", getLogLevelName(LogLevel::FETAL), message, __FILE__, __LINE__);\
        std::exit(EXIT_FAILURE);\
    }\
}
#define BMH_LOG(expr, logLevel) {\
    LogLevel level = logLevel;\
    std::println("{}: {} at {}:{}", getLogLevelName(level), expr, __FILE__, __LINE__);\
    if(level == LogLevel::FETAL)\
        std::exit(EXIT_FAILURE);\
}
#define BMH_INFO(expr) {\
    BMH_LOG(expr, LogLevel::INFO);\
}
#define BMH_TRACE(expr) {\
    BMH_LOG(expr, LogLevel::TRACE);\
}
#define BMH_WARNING(expr) {\
    BMH_LOG(expr, LogLevel::WARNING);\
}
#define BMH_ERROR(expr) {\
    BMH_LOG(expr, LogLevel::ERROR);\
}
#endif

///////////////////////

///////////////////////

enum CoralWindowFlag {
    CoralWindowFlag_None = 0,
    CoralWindowFlag_Fullscreen = 1 << 0,
    CoralWindowFlag_Iconified = 1 << 1,
    CoralWindowFlag_Resizable = 1 << 2,
    CoralWindowFlag_NoBorder = 1 << 3,
};

enum CoralRenderFlag {
    CoralRenderFlag_None = 0,
    CoralRenderFlag_Batch = 1 << 0,
    CoralRenderFlag_Discrete = 1 << 1,
    CoralRenderFlag_RecreateSwapChain = 1 << 2,
};

enum CoralEventFlag {
    CoralEventFlag_None = 0,
    CoralEventFlag_WindowResize = 1 << 0,
    CoralEventFlag_WindowClose = 1 << 1,
    CoralEventFlag_WindowFocus = 1 << 2,
    CoralEventFlag_WindowLostFocus = 1 << 3,

    CoralEventFlag_KeyPressed = 1 << 4,
    CoralEventFlag_KeyReleased = 1 << 5,
    CoralEventFlag_KeyTyped = 1 << 6,
    CoralEventFlag_KeyRepeat = 1 << 7,

    CoralEventFlag_MouseButtonPressed = 1 << 8,
    CoralEventFlag_MouseButtonReleased = 1 << 9,
    CoralEventFlag_MouseMove = 1 << 10,
    CoralEventFlag_MouseScroll = 1 << 11,

    CoralEventFlag_Input = 1 << 12,
};


namespace coral {
    using FLAG = int;
} // namespace coral