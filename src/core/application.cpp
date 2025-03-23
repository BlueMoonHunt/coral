#include "application.hpp"

#include "renderer/renderer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace coral {
    Application* Application::s_Instance = nullptr;

    Application::Application(const ApplicationSpecifications& specifications)
        : m_Specs(specifications) {
        s_Instance = this;

        m_Window.create(m_Specs.windowsize, m_Specs.name, m_Specs.windowFlags);

        m_Renderer = std::make_shared<Renderer>();
        // m_Renderer->init(m_Window.getNativeWindow());
        initCallbacks();
    }

    Application::~Application() {
        m_Window.destroy();
        // m_Renderer->shutdown();
        glfwTerminate();
    }

    void Application::run() {

        m_Specs.running = true;

        while (!glfwWindowShouldClose(m_Window.getNativeWindow()) && m_Specs.running) {
            glfwPollEvents();

            while (!m_EventQueue.empty()) {
                auto e = m_EventQueue.front();
                m_EventQueue.pop();
                onEvent(e);
            }

            glClearColor(0.38f, 0.3f, 0.6f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(m_Window.getNativeWindow());


            m_Renderer->beginFrame();

            m_Renderer->endFrame();
        }
    }

    void Application::onEvent(Ref<Event> e) {

        if (e->flag & CoralEventFlag_WindowResize) {
            auto event = std::dynamic_pointer_cast<WindowResizeEvent>(e);
            if (event)
                m_Specs.windowsize = event->getSize();
        }
    }

    void Application::initCallbacks() {
        GLFWwindow* windowHandle = m_Window.getNativeWindow();
        glfwSetErrorCallback([](int error, const char* description) {
            fprintf(stderr, "GLFW Error %d: %s\n", error, description);
            });

        glfwSetWindowSizeCallback(windowHandle, [](GLFWwindow* m_Window, int width, int height) {
            const i32vec2_t windowSize = s_Instance->m_Specs.windowsize;
            if (windowSize.x != width, windowSize.y != height)
                s_Instance->m_EventQueue.emplace(CreateRef<WindowResizeEvent>(width, height));
            });

        glfwSetKeyCallback(windowHandle, [](GLFWwindow* m_Window, int key, int scancode, int action, int mods) {
            switch (action) {
            case GLFW_PRESS: s_Instance->m_EventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, false)); break;
            case GLFW_RELEASE: s_Instance->m_EventQueue.emplace(CreateRef<KeyReleasedEvent>((KeyCode)key)); break;
            case GLFW_REPEAT: s_Instance->m_EventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, true)); break;
            }
            });

        glfwSetWindowCloseCallback(windowHandle, [](GLFWwindow* m_Window) {
            s_Instance->m_EventQueue.push(CreateRef<WindowCloseEvent>());
            });

        glfwSetCharCallback(windowHandle, [](GLFWwindow* m_Window, uint32_t keycode) {
            s_Instance->m_EventQueue.emplace(CreateRef<KeyTypedEvent>((KeyCode)keycode));
            });

        glfwSetMouseButtonCallback(windowHandle, [](GLFWwindow* m_Window, int button, int action, int modes) {
            switch (action) {
            case GLFW_PRESS:    s_Instance->m_EventQueue.push(CreateRef<MouseButtonPressedEvent>((MouseButton)button)); break;
            case GLFW_RELEASE:  s_Instance->m_EventQueue.push(CreateRef<MouseButtonReleasedEvent>((MouseButton)button)); break;
            }
            });

        glfwSetScrollCallback(windowHandle, [](GLFWwindow* m_Window, double xOffset, double yOffset) {
            s_Instance->m_EventQueue.push(CreateRef<MouseScrolledEvent>((float)xOffset, (float)yOffset));
            });

        glfwSetCursorPosCallback(windowHandle, [](GLFWwindow* m_Window, double xPos, double yPos) {
            s_Instance->m_EventQueue.push(CreateRef<MouseMovedEvent>(xPos, yPos));
            });
    }
}