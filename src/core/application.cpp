#include "application.h"

#include <print>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event/event.h"
#include "timestep.h"
#include "coralLogger.h"
#include "util/customTitleBar.h"

namespace coral {

    Application* Application::s_Instance = nullptr;

    Application::Application(const char* title, uint32_t width, uint32_t height, ApplicationFlag flags)
        : m_Title(title), m_Width(width), m_Height(height), m_Flags(flags) {
        s_Instance = this;
        init();
    }

    Application::~Application() {
        close();
    }

    void Application::run() {

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            float time = (float)glfwGetTime();
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;


            while (!Application::Get().m_EventQueue.empty()) {
                auto e = Application::Get().m_EventQueue.front();
                Application::Get().m_EventQueue.pop();
                onEvent(e);
            }

            if (m_Flags & ApplicationFlag_Debug)
                Log::printLogs();

            glfwSwapBuffers(glfwGetCurrentContext());
        }
    }

    // Events

    bool Application::onWindowClose(WindowCloseEvent& e) {
        return true;
    }

    bool Application::onWindowResize(WindowResizeEvent& e) {
        auto& app = Application::Get();
        app.m_Width = e.getWidth();
        app.m_Height = e.getHeight();
        return false;
    }

    bool Application::onKeyPressed(KeyPressedEvent& e) {
        return true;
    }

    bool Application::onKeyReleased(KeyReleasedEvent& e) {
        return true;
    }

    bool Application::onKeyTyped(KeyTypedEvent& e) {
        return true;
    }

    bool Application::onMouseMoved(MouseMovedEvent& e) {
        return true;
    }

    bool Application::onMouseScrolled(MouseScrolledEvent& e) {
        return true;
    }

    bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) {
        return true;
    }

    bool Application::onMouseButtonReleased(MouseButtonReleasedEvent& e) {
        return true;
    }

    bool Application::onMouseDragged(MouseDraggedEvent& e) {
        glfwSetWindowPos(Application::Get().m_Window, e.getX(), e.getY());
        return true;
    }

    void Application::onEvent(Ref<Event> e) {
        EventDispatcher dispatcher(*e);

        if (e->isInCategory(EventCategoryMouseButton)) {
            dispatcher.dispatch<MouseButtonPressedEvent>(onMouseButtonPressed);
            dispatcher.dispatch<MouseButtonReleasedEvent>(onMouseButtonReleased);
            dispatcher.dispatch<MouseDraggedEvent>(onMouseDragged);
        }
        else if (e->isInCategory(EventCategoryMouse)) {
            dispatcher.dispatch<MouseMovedEvent>(onMouseMoved);
            dispatcher.dispatch<MouseScrolledEvent>(onMouseScrolled);

        }
        if (e->isInCategory(EventCategoryInput)) {
            dispatcher.dispatch<KeyTypedEvent>(onKeyTyped);
        }

        if (e->isInCategory(EventCategoryApplication)) {
            dispatcher.dispatch<WindowCloseEvent>(onWindowClose);
            dispatcher.dispatch<WindowResizeEvent>(onWindowResize);
        }


        if (e->isInCategory(EventCategoryKeyboard)) {
            dispatcher.dispatch<KeyPressedEvent>(onKeyPressed);
            dispatcher.dispatch<KeyReleasedEvent>(onKeyReleased);
        }
    }

    void Application::setWindowTitle(const char* title) {
        glfwSetWindowTitle(m_Window, title);
    }

    void Application::setWindowSize(uint32_t width, uint32_t height) {
        width = width < 1 ? 1 : width;
        height = height < 1 ? 1 : height;

        if (m_Width != width || m_Height != height)
            glfwSetWindowSize(m_Window, width, height);
    }

    void Application::setWindowPosition(int x, int y) {
        glfwSetWindowPos(m_Window, x, y);
    }

    void Application::setWindowOpacity(float opacity) {
        glfwSetWindowOpacity(m_Window, std::clamp(opacity, 0.0f, 1.0f));
    }


    void Application::init() {
        if (!glfwInit() && (m_Flags & ApplicationFlag_Debug))
            std::println("Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        if (m_Flags & ApplicationFlag_NoResize)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        if (m_Flags & ApplicationFlag_Fullscreen)
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && (m_Flags & ApplicationFlag_Debug))
            std::println("Failed to initialize GLAD");

        if (m_Flags & ApplicationFlag_VSync)
            glfwSwapInterval(1);

        initCallbacks();


    }

    void Application::initCallbacks() {
        glfwSetErrorCallback([](int error, const char* description) {
            fprintf(stderr, "GLFW Error %d: %s\n", error, description);
            });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            auto& app = Application::Get();
            if (app.m_Width != width || app.m_Height != height)
                app.m_EventQueue.emplace(CreateRef<WindowResizeEvent>(width, height));
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
            Application::Get().m_EventQueue.emplace(CreateRef<WindowCloseEvent>());
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scanCode, int action, int modes) {
            switch (action) {
            case GLFW_PRESS:    Application::Get().m_EventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, false)); break;
            case GLFW_RELEASE:  Application::Get().m_EventQueue.emplace(CreateRef<KeyReleasedEvent>((KeyCode)key)); break;
            case GLFW_REPEAT:   Application::Get().m_EventQueue.emplace(CreateRef<KeyPressedEvent>((KeyCode)key, true)); break;
            }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode) {
            Application::Get().m_EventQueue.emplace(CreateRef<KeyTypedEvent>((KeyCode)keycode));
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int modes) {
            switch (action) {
            case GLFW_PRESS:    Application::Get().m_EventQueue.emplace(CreateRef<MouseButtonPressedEvent>((MouseButton)button)); break;
            case GLFW_RELEASE:  Application::Get().m_EventQueue.emplace(CreateRef<MouseButtonReleasedEvent>((MouseButton)button)); break;
            }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Application::Get().m_EventQueue.emplace(CreateRef<MouseScrolledEvent>((float)xOffset, (float)yOffset));
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                Application::Get().m_EventQueue.emplace(CreateRef<coral::MouseDraggedEvent>(static_cast<float>(xPos), static_cast<float>(yPos), MouseButton::Left));
                Application::Get().m_EventQueue.emplace(CreateRef<coral::MouseMovedEvent>(static_cast<float>(xPos), static_cast<float>(yPos)));
            }
            else {
                Application::Get().m_EventQueue.emplace(CreateRef<coral::MouseMovedEvent>(static_cast<float>(xPos), static_cast<float>(yPos)));
            }
            });
    }

    void Application::close() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

} // namespace coral