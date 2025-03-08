#include "core/application.hpp"

int main() {
    coral::Application* app = new coral::Application(coral::ApplicationProperties{
        .name = "coral",
        .layout{900,600},
        .applicationFlags = CoralApplicationFlag_Resizable,
    });

    app->run();
    
    delete app;
}