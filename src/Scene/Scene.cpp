#include "Scene.h"

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

#include "Camera.h"
#include "EffectDescription.h"
#include "Entity.h"
#include "GameInputContext.h"
#include "Graphics.h"
#include "LookScript.h"
#include "ModelReader.h"
#include "MoveScript.h"
#include "PhysicalInput.h"
#include "Property.h"
#include "RenderObjectData.h"
#include "ScriptSystem.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Vector3.h"

Scene::Scene() {
    setupSystems();

    camera = Camera::create(60, 16.f / 9, 0.1, 1000);

    auto renderer = Graphics::getRenderEngine();

    RenderObjectData tower_data =
        *readRenderObject("./Assets/Scene/Tower.json");
    renderer->getRenderWorld().addOpaqueObject(tower_data);

    RenderObjectData gem_data = *readRenderObject("./Assets/Scene/Gem.json");
    renderer->getRenderWorld().addOpaqueObject(gem_data);

    RenderObjectData island_data =
        *readRenderObject("./Assets/Scene/Island.json");
    renderer->getRenderWorld().addOpaqueObject(island_data);

    auto orb = *readEffect("./Assets/Scene/Effects/Orb.json");
    renderer->getRenderWorld().addEffect(orb);
    auto lightning1 = *readEffect("./Assets/Scene/Effects/Lightning1.json");
    renderer->getRenderWorld().addEffect(lightning1);
    auto lightning3 = *readEffect("./Assets/Scene/Effects/Lightning3.json");
    renderer->getRenderWorld().addEffect(lightning3);
    auto lightning4 = *readEffect("./Assets/Scene/Effects/Lightning4.json");
    renderer->getRenderWorld().addEffect(lightning4);

    auto mesh_effect = Graphics::MeshEffectDescription{};
    mesh_effect.center = {0, 0, 0};
    mesh_effect.extents = {0, 0, 0};
    mesh_effect.spawn_rate = 1;
    mesh_effect.particle_lifetime = 1;
    mesh_effect.mesh = 0;
    // renderer->addMesh
    // (File::ModelReader("./Assets/Sphere.fbx").readMesh());
    // mesh_effect.texture = *readTexture("./Assets/Tower.png");
    renderer->getRenderWorld().addMeshEffect(mesh_effect);

    auto input = std::make_shared<Input::GameInputContext>();
    input->addBinding(Input::GameAxes::LookYaw, Input::Axis::MOUSE_X);
    input->addBinding(Input::GameAxes::LookPitch, Input::Axis::MOUSE_Y);
    input->addBinding(Input::GameAxes::MoveForwardBackward,
                      Input::Button::KEYBOARD_W, 1);
    input->addBinding(Input::GameAxes::MoveForwardBackward,
                      Input::Button::KEYBOARD_S, -1);
    input->addBinding(Input::GameAxes::MoveLeftRight, Input::Button::KEYBOARD_D,
                      1);
    input->addBinding(Input::GameAxes::MoveLeftRight, Input::Button::KEYBOARD_A,
                      -1);
    input->addBinding(Input::GameAxes::MoveUpDown,
                      Input::Button::KEYBOARD_LSHIFT, 1);
    input->addBinding(Input::GameAxes::MoveUpDown,
                      Input::Button::KEYBOARD_LCTRL, -1);

    Entity player = world.createEntity();
    Entity head = world.createEntity();
    head.set<Transform>({});
    player.addChild(head);
    player.set<Transform>({});
    player.addScript(std::make_unique<LookScript>(head, player, camera, input));
    player.addScript(std::make_unique<MoveScript>(player, input));
}

void Scene::update(float deltaTime) {
    ZoneScoped;

    sky.draw();
    world.update(deltaTime);

    auto renderer = Graphics::getRenderEngine();
    renderer->getRenderWorld().update(deltaTime);
}

void Scene::setupSystems() {
    world.addSystem<TransformSystem>();
    world.addSystem<ScriptSystem>();
}

std::optional<RenderObjectData> Scene::readRenderObject(
    const std::filesystem::path& path) {
    auto file = std::ifstream(path);

    auto data = nlohmann::json();
    file >> data;

    auto mesh_path = data["mesh"].get<std::string>();
    auto albedo_path = data["albedo"].get<std::string>();
    auto position = data["position"].get<Vector3>();

    auto mesh_data = File::ModelReader(mesh_path).readMesh();
    auto renderer = Graphics::getRenderEngine();
    auto albedo = *readTexture(albedo_path);
    auto mesh = renderer->addMesh(mesh_data);

    return RenderObjectData{position, albedo, mesh};
}

std::optional<Graphics::EffectDescription> Scene::readEffect(
    const std::filesystem::path& path) {
    auto file = std::ifstream(path);

    auto data = nlohmann::json();
    file >> data;

    auto effect = Graphics::EffectDescription{};
    data.at("center").get_to(effect.center);
    data.at("extents").get_to(effect.extents);
    data.at("spawn_rate").get_to(effect.spawn_rate);
    data.at("color").get_to(effect.color);
    data.at("size").get_to(effect.size);
    data.at("rotation").get_to(effect.rotation);
    data.at("lifetime").get_to(effect.particle_lifetime);

    auto texture_path = data["texture"].get<std::string>();
    effect.texture = *readTexture(texture_path);

    return effect;
}
