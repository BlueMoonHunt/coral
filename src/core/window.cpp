#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace coral {
    void Window::create(i32vec2_t size, const char* title, FLAG windowFlags) {
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

        m_Window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        // Initialize glad after m_Window creation
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        const char* description;
        glfwGetError(&description);

        if (!m_Window) {
            std::cerr << "GLFW m_Window creation failed!\n" << description << std::endl;
            glfwTerminate();
            return;
        }

        if (windowFlags & CoralWindowFlag_Iconified)
            glfwIconifyWindow(m_Window);

        glfwSwapInterval(1);
    }

    void Window::destroy() {
        glfwDestroyWindow(m_Window);
    }

    bool Window::shouldClose() {
        return glfwWindowShouldClose(m_Window);
    }

    GLFWwindow* Window::getNativeWindow() {
        return m_Window;
    }


} // namespace coral
