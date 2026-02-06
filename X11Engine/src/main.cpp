#include "Core.h"
#include "Window.h"

int main(int argc, char* argv[]) {
    Core::init();
    Window::get().processMessages();
}
