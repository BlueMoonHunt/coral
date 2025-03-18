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
        ApplicationSpecifications& getProperties() { return specs; }
        Window getWindow() { return window; }
        void run();
    private:
        void onEvent(Ref<Event> e);
        void initCallbacks();
    private:
        static Application* s_Instance;
        ApplicationSpecifications specs;
        Window window;
        Ref<Renderer> renderer;
        std::queue<Ref<Event>> eventQueue;
    };
} // namespace coral