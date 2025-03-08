#include "core/application.hpp"

int main() {
    coral::Application* app = new coral::Application(coral::ApplicationSpecifications{
        .windowsize{900,600},
        .windowFlags = CoralWindowFlag_None,
    });

    app->run();
    
    delete app;
}