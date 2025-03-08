#pragma once
#include "base.hpp"
#include <memory>
#include "window.hpp"

struct GLFWwindow;

namespace coral {
    class Renderer;

    struct ApplicationSpecifications {
        const char* name = "CORAL APP";
        ivec2 windowsize = { 100,100 };
        FLAG windowFlags = CoralWindowFlag_None;
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
        void initCallbacks();
    private:
        static Application* s_Instance;
        ApplicationSpecifications specs;
        Window window;
        std::shared_ptr<Renderer> renderer;
    };
} // namespace coral