#include "Core.h"
#include "Engine.h"
#include "Scene.h"

int main(int argc, char* argv[]) {
    if (Core::init() == false) return -1;

    Engine::Engine::get().init();
    Engine::Engine::get().run(new Scene());
}
