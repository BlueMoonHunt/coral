#include "application.hpp"
#include <GLFW/glfw3.h>
#include <print>

namespace coral {
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationProperties& props)
        : properties(props) {
        s_Instance = this;
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, (properties.applicationFlags & CoralApplicationFlag_Resizable) ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, (properties.applicationFlags & CoralApplicationFlag_NoBorder) ? GLFW_FALSE : GLFW_TRUE);

        if (properties.applicationFlags & CoralApplicationFlag_Fullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            properties.layout = { mode->width, mode->height };
        }

        window = glfwCreateWindow(properties.layout.x, properties.layout.y, properties.name, nullptr, nullptr);
        glfwSetWindowUserPointer(window, s_Instance);

        if (properties.applicationFlags & CoralApplicationFlag_Iconified)
            glfwIconifyWindow(window);

        renderer.init(window);
        initCallbacks();
    }

    Application::~Application() {
        glfwDestroyWindow(window);
        renderer.shutdown();
        glfwTerminate();
    }

    void Application::run() {
        while (!glfwWindowShouldClose(window)) {
            renderer.beginFrame();

            renderer.endFrame();
        }
    }

    void Application::initCallbacks() {
        auto framebuffersizeCallbackFunction = [](GLFWwindow* window, int width, int height) {
            auto& rendererProps = s_Instance->renderer.getProperties();
            rendererProps.framebufferSize = { width,height };
            if (!(rendererProps.flags & CoralRendererFlag_RecreateSwapChain))
                rendererProps.flags |= CoralRendererFlag_RecreateSwapChain;
            };

        // functions to run in case callback can't run on initialization
        ivec2 size;
        glfwGetFramebufferSize(window, &size.x, &size.y);
        framebuffersizeCallbackFunction(window, size.x, size.y);

        glfwSetFramebufferSizeCallback(window, framebuffersizeCallbackFunction);
    }
}