#include "App.hpp"

#include <utility>

App::App(DAppManifest manifest, DSettings settings)
: settings(std::move(settings)), manifest(std::move(manifest)){
    render = _new(RenderManager,
        manifest,
        settings.graphics
    );
    inp = new TInput(render->window->self);
}

App::~App() {
    delete inp;
}

void App::run() {
    while (!render->window->isEnd()) {
        if (inpt key(TInput::EKey::ESCAPE, TInput::EKeyAction::PRESS))
            render->window->end();

        glfwPollEvents();
    }
}