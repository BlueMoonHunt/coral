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

        specs.running = true;

        while (!glfwWindowShouldClose(window.getNativeWindow()) && specs.running) {
            glfwPollEvents();

            while (!eventQueue.empty()) {
                auto e = eventQueue.front();
                eventQueue.pop();
                onEvent(e);
            }

            glClearColor(0.38f, 0.3f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(window.getNativeWindow());


            renderer->beginFrame();

            renderer->endFrame();
        }
    }

    void Application::onEvent(Ref<Event> e) {

        if (e->flag & CoralEventFlag_WindowResize) {
            auto event = std::dynamic_pointer_cast<WindowResizeEvent>(e);
            if (event)
                specs.windowsize = event->getSize();
        }
    }

    void Application::initCallbacks() {
        GLFWwindow* windowHandle = window.getNativeWindow();
        glfwSetErrorCallback([](int error, const char* description) {
            fprintf(stderr, "GLFW Error %d: %s\n", error, description);
            });

        glfwSetWindowSizeCallback(windowHandle, [](GLFWwindow* window, int width, int height) {
            const i32vec2_t windowSize = s_Instance->specs.windowsize;
            if (windowSize.x != width, windowSize.y != height)
                s_Instance->eventQueue.emplace(CreateRef<WindowResizeEvent>(width, height));
            });

        glfwSetKeyCallback(windowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            switch (action) {
            case GLFW_PRESS: s_Instance->eventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, false)); break;
            case GLFW_RELEASE: s_Instance->eventQueue.emplace(CreateRef<KeyReleasedEvent>((KeyCode)key)); break;
            case GLFW_REPEAT: s_Instance->eventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, true)); break;
            }
            });

        glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* window) {
            s_Instance->eventQueue.push(CreateRef<WindowCloseEvent>());
            });

        glfwSetCharCallback(windowHandle, [](GLFWwindow* window, uint32_t keycode) {
            s_Instance->eventQueue.emplace(CreateRef<KeyTypedEvent>((KeyCode)keycode));
            });

        glfwSetMouseButtonCallback(windowHandle, [](GLFWwindow* window, int button, int action, int modes) {
            switch (action) {
            case GLFW_PRESS:    s_Instance->eventQueue.push(CreateRef<MouseButtonPressedEvent>((MouseButton)button)); break;
            case GLFW_RELEASE:  s_Instance->eventQueue.push(CreateRef<MouseButtonReleasedEvent>((MouseButton)button)); break;
            }
            });

        glfwSetScrollCallback(windowHandle, [](GLFWwindow* window, double xOffset, double yOffset) {
            s_Instance->eventQueue.push(CreateRef<MouseScrolledEvent>((float)xOffset, (float)yOffset));
            });

        glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* window, double xPos, double yPos) {
            s_Instance->eventQueue.push(CreateRef<MouseMovedEvent>(xPos, yPos));
            });
    }
}