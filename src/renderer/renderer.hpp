#pragma once
#include "base.hpp"

struct GLFWwindow;
namespace coral {

    class Renderer {
    public:
        void init(GLFWwindow* window);
        void shutdown();

        void beginFrame();
        void endFrame();
    private:
    private:
        FLAG flag;
    };
} // namespace coral
