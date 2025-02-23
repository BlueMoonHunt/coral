#include "core/application.h"

int main() {
    coral::Application* app = new coral::Application(
        "Hello, World!",
        800, 600,
        ApplicationFlag_Debug | ApplicationFlag_VSync
    );
    
    app->run();
    delete app;
    return 0;
}
