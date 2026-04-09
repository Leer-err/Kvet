#include "Core.h"
#include "Engine.h"

int main(int argc, char* argv[]) {
    Core::init();

    Engine::Engine::get().run();
}
