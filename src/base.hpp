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
#define CORAL_LOG(expr, logLevel) {expr;}
#else
#define CORAL_LOG(expr, logLevel) {\
    LogLevel level = logLevel;\
    std::println("{}: {}", getLogLevelName(level), expr, __FILE__, __LINE__);\
    if(level == LogLevel::FETAL)\
        exit(EXIT_FAILURE);\
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