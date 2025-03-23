#pragma once
#include "base.hpp"
#include <memory>
#include <queue>
#include "window.hpp"
#include "event/event.hpp"

struct GLFWwindow;

namespace coral {
    class Renderer;

    struct ApplicationSpecifications {
        const char* name = "CORAL APP";
        i32vec2_t windowsize = { 100,100 };
        FLAG windowFlags = CoralWindowFlag_None;
        bool running = false;
    };

    class Application {
    public:
        Application(const ApplicationSpecifications& specs);
        ~Application();

        inline static Application* Get() { return s_Instance; }
        ApplicationSpecifications& getProperties() { return m_Specs; }
        Window getWindow() { return m_Window; }
        void run();
    private:
        void onEvent(Ref<Event> e);
        void initCallbacks();
    private:
        static Application* s_Instance;
        ApplicationSpecifications m_Specs;
        Window m_Window;
        Ref<Renderer> m_Renderer;
        std::queue<Ref<Event>> m_EventQueue;
    };
} // namespace coral