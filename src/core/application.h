#pragma once
#include <queue>

#include "base.h"
#include "event/event.h"

struct GLFWwindow;

namespace coral {

    class Application {
    public:
        Application(const char* title, uint32_t width, uint32_t height, ApplicationFlag flags);
        ~Application();

        inline static Application& Get() { return *s_Instance; }

        void run();

        void onEvent(Ref<Event> e);

        void setWindowTitle(const char* title);
        void setWindowSize(uint32_t width, uint32_t height);
        void setWindowIcon(const char* path);
        void setWindowPosition(int x, int y);
        void setWindowOpacity(float opacity);

        inline int getApplicationFlags() const { return m_Flags; }


    private:
        void init();
        void initCallbacks();
        void close();
        // Events
        static bool onWindowClose(WindowCloseEvent& e);
        static bool onWindowResize(WindowResizeEvent& e);
        static bool onKeyPressed(KeyPressedEvent& e);
        static bool onKeyReleased(KeyReleasedEvent& e);
        static bool onKeyTyped(KeyTypedEvent& e);
        static bool onMouseMoved(MouseMovedEvent& e);
        static bool onMouseScrolled(MouseScrolledEvent& e);
        static bool onMouseButtonPressed(MouseButtonPressedEvent& e);
        static bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
        static bool onMouseDragged(MouseDraggedEvent& e);

    private:
        GLFWwindow* m_Window;
        static Application* s_Instance;

        std::queue<Ref<Event>> m_EventQueue;
        float m_LastFrameTime;
        const char* m_Title;
        uint32_t m_Width, m_Height;
        ApplicationFlag m_Flags;
        uint32_t m_TitleBarHeight = 30;
    };
} // namespace coral