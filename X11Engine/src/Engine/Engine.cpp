#include "Engine.h"

#include <chrono>
#include <thread>
#include <tracy/Tracy.hpp>

#include "Graphics.h"
#include "ModelReader.h"
#include "Scene.h"
#include "StaticModelData.h"
#include "Window.h"

// #include "GameInputConfigReader.h"
// #include "Overlay.h"
#include "PhysicalInput.h"
// #include "ScriptLoader.h"
// #include "ScriptSandbox.h"
#include <stb_image.h>

#include "GraphicsCommunicationManager.h"

namespace Engine {

Engine::Engine() : should_exit(false) {}

bool Engine::init() {
    // RenderEngine::get().initializeResources();

    // Scene::get();
    // ::Engine::Script::ScriptLoader().loadFromDirectory(
    //     "E:\\repos\\X11Engine\\X11Engine\\Scripts");
    // Script::ScriptSandbox::get().runFunction("Init");

    // Overlay::Overlay::get().add<Overlay::OverlayElements::Text>(
    //     "General", [this]() { return std::format("FPS is {}", fps); });

    return true;
}

StaticModelData model_data = {};

void Engine::run() {
    // GameInputConfigReader input_config_reader;
    // input_config_reader.read(
    //     "E:\\repos\\X11Engine\\X11Engine\\src\\Data\\Input\\Config.json",
    //     GameInputContext::get());

    renderer = Graphics::getRenderEngine();

    File::ModelReader reader("./Assets/Gem.fbx");
    auto vertices = reader.readVertices();
    auto indices = reader.readIndices();

    model_data.position = {0, 0, 10};
    model_data.mesh =
        renderer->addMesh(vertices.data(), vertices.size() * sizeof(Vertex),
                          indices.data(), indices.size() * sizeof(uint32_t));

    int width;
    int height;
    int channels;
    unsigned char* data =
        stbi_load("./Assets/Gem.png", &width, &height, &channels, 0);

    model_data.albedo = renderer->addTexture(data, width, height);

    main_loop_thread = std::thread([this]() { mainLoopWorker(); });

    windowWorker();

    main_loop_thread.join();
}

void Engine::windowWorker() {
    while (Window::get().processMessages()) {
    }

    should_exit = true;
}

void Engine::mainLoopWorker() {
    last_elapsed = clock.now();
    start = clock.now();

    while (should_exit == false) {
        auto elapsed = clock.now();
        float delta_time =
            std::chrono::duration_cast<std::chrono::duration<float>>(
                elapsed - last_elapsed)
                .count();
        last_elapsed = elapsed;

        update(delta_time);
    }
}

void Engine::update(float delta_time) {
    ZoneScoped;

    fps = 1.f / delta_time;

    Input::PhysicalInput::get().saveState();

    Scene::get().update(delta_time);

    GraphicsCommunicationManager::get().send(model_data);
    renderer->render();

    GraphicsCommunicationManager::get().clear();

    // Overlay::Overlay::get().draw();

    FrameMark;
}

void Engine::exit() { should_exit = true; }

float Engine::getTime() const {
    auto elapsed = clock.now();
    return std::chrono::duration_cast<std::chrono::duration<float>>(elapsed -
                                                                    start)
        .count();
}

};  // namespace Engine