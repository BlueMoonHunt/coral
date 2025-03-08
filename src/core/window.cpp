#include "window.hpp"

#include <GLFW/glfw3.h>

namespace coral {
    void Window::create(ivec2 size, const char* title, FLAG windowFlags) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, (windowFlags & CoralWindowFlag_Resizable) ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, (windowFlags & CoralWindowFlag_NoBorder) ? GLFW_FALSE : GLFW_TRUE);

        if (windowFlags & CoralWindowFlag_Fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            size = { mode->width, mode->height };
        }

        window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);

        if (windowFlags & CoralWindowFlag_Iconified)
            glfwIconifyWindow(window);
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
