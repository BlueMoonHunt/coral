#pragma once
#include "base.hpp"
#include "renderer/renderer.hpp"

struct GLFWwindow;

namespace coral {
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
        Renderer& getRenderer() { return renderer; }

        void run();
    private:
        void initCallbacks();
    private:
        ApplicationProperties properties;
        Renderer renderer;
        GLFWwindow* window;
        static Application* s_Instance;
    };
} // namespace coral