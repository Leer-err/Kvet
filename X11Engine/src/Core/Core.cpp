#include "Core.h"

#include "Renderer.h"
#include "Window.h"

namespace Core {

void init() {
    Window::get().init();
    Graphics::Renderer::get();

    Window::get().processMessages();
}

}  // namespace Core