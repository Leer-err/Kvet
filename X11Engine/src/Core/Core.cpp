#include "Core.h"

#include "Graphics.h"
#include "Window.h"

namespace Core {

bool init() {
    Window::get().init();
    if (Graphics::init() == false) return false;

    return true;
}

}  // namespace Core