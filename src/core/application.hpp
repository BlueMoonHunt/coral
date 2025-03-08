#pragma once
#include "base.hpp"
#include <memory>

struct GLFWwindow;

namespace coral {
    class Renderer;

    struct ApplicationProperties {
        const char* name = "";
        ivec2 layout = { 100,100 };
        FLAG applicationFlags = CoralApplicationFlag_None;
    };

    class Application {
    public:
        Application(const ApplicationProperties& props);
        ~Application();

        inline static Application* Get() { return s_Instance; }
        ApplicationProperties& getProperties() { return properties; }
        void run();
    private:
        void initCallbacks();
    private:
        ApplicationProperties properties;
        std::shared_ptr<Renderer> renderer;
        GLFWwindow* window;
        static Application* s_Instance;
    };
} // namespace coral