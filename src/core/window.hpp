#pragma once
#include "base.hpp"

struct GLFWwindow;

namespace coral {
    class Window {
    public:
        void create(ivec2 size, const char* title, FLAG windowFlags);
        void destroy();
        bool shouldClose();
        GLFWwindow* getNativeWindow();
    private:
        GLFWwindow* window;
    };
} // namespace coral
