#include "application.hpp"

#include "renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace coral {
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecifications& specifications)
        : specs(specifications) {
        s_Instance = this;

        window.create(specs.windowsize, specs.name, specs.windowFlags);

        renderer = std::make_shared<Renderer>();
        // renderer->init(window.getNativeWindow());
        initCallbacks();
    }

    Application::~Application() {
        window.destroy();
        // renderer->shutdown();
        glfwTerminate();
    }

    void Application::run() {
        while (!glfwWindowShouldClose(window.getNativeWindow())) {
            glfwPollEvents();

            glClearColor(0.38f,0.3f,0.6f,1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window.getNativeWindow());


            // renderer->beginFrame();

            // renderer->endFrame();
        }
    }

    void Application::initCallbacks() {
    }
}