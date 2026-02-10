#include "AppConfig.h"

#include "GraphicsConfig.h"
#include "WindowConfig.h"

namespace Config {

App::App() { loadDefaults(); }

void App::loadDefaults() {
    graphics.render_width = 1280;
    graphics.render_height = 720;

    graphics.buffering_mode = BufferingMode::NoBuffering;

    window.width = 1280;
    window.height = 720;

    window.state = Config::Window::State::Windowed;
}

Graphics App::getGraphicsConfig() { return graphics; }

Window App::getWindowConfig() { return window; }

};  // namespace Config