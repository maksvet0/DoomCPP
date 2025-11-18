#include "App.hpp"

#include <utility>

App::App(DAppManifest manifest, DSettings settings)
: settings(settings), manifest(manifest){
    dbg = _new(TDebug, this->settings.debug);
    render = new RenderManager(
        manifest,
        settings.graphics
    );
    inp = new TInput(render->window->self);
}

App::~App() {
    delete inp;
    delete render;
}

void App::run() {
    while (!render->window->isEnd()) {
        if (inpt key(TInput::EKey::ESCAPE, TInput::EKeyAction::PRESS))
            render->window->end();

        render->update();
    }
    render->waitForEnd();
}