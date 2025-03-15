#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace coral {
    void Window::create(ivec2 size, const char* title, FLAG windowFlags) {
        if (!glfwInit()) {
            std::cerr << "GLFW initialization failed!" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, (windowFlags & CoralWindowFlag_Resizable) ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, (windowFlags & CoralWindowFlag_NoBorder) ? GLFW_FALSE : GLFW_TRUE);

        if (windowFlags & CoralWindowFlag_Fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            size = { mode->width, mode->height };
        }

        window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
        glfwMakeContextCurrent(window);

        // Initialize glad after window creation
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        const char* description;
        glfwGetError(&description);

        if (!window) {
            std::cerr << "GLFW window creation failed!\n" << description << std::endl;
            glfwTerminate();
            return;
        }

        if (windowFlags & CoralWindowFlag_Iconified)
            glfwIconifyWindow(window);

        glfwSwapInterval(1);
    }

    void Window::destroy() {
        glfwDestroyWindow(window);
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    GLFWwindow* Window::getNativeWindow() {
        return window;
    }


} // namespace coral
