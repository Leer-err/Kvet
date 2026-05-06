#include "Core.h"
#include "Engine.h"

int main(int argc, char* argv[]) {
    if (Core::init() == false) return -1;

    Engine::Engine::get().run();
}
