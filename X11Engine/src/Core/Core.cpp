#include "Core.h"

#include "Graphics.h"
#include "Window.h"

namespace Core {

void init() {
    Window::get().init();
    Graphics::init();
}

}  // namespace Core