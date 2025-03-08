#include "application.hpp"

#include "renderer/renderer.hpp"

#include <GLFW/glfw3.h>

namespace coral {
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecifications& specifications)
        : specs(specifications) {
        s_Instance = this;
        
        window.create(specs.windowsize,specs.name, specs.windowFlags);

        renderer = std::make_shared<Renderer>();
        renderer->init(window.getNativeWindow());
        initCallbacks();
    }

    Application::~Application() {
        window.destroy();
        renderer->shutdown();
        glfwTerminate();
    }

    void Application::run() {
        while (!window.shouldClose()) {
            glfwPollEvents();

            renderer->beginFrame();

            renderer->endFrame();
        }
    }

    void Application::initCallbacks() {
    }
}