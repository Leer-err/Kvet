#include "Scene.h"

#include <cstddef>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <tracy/Tracy.hpp>
#include <vector>

#include "AssetManager.h"
#include "Camera.h"
#include "CustomRenderObject.h"
#include "EffectDescription.h"
#include "Entity.h"
#include "Filesystem.h"
#include "GameInputContext.h"
#include "Graphics.h"
#include "Handle.h"
#include "LookAtScript.h"
#include "LookScript.h"
#include "Matrix.h"
#include "MoveScript.h"
#include "PhysicalInput.h"
#include "Property.h"
#include "RenderObjectData.h"
#include "ScriptSystem.h"
#include "StarsScript.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Vector3.h"

static Graphics::CustomRenderObject handle;

Scene::Scene() {
    setupSystems();

    camera = Camera::create(60, 16.f / 9, 0.1, 1000);

    auto scene = File::Filesystem::getScene("Scene");

    parseScene(scene.getResult());

    // auto mesh_effect = Graphics::MeshEffectDescription{};
    // mesh_effect.center = {0, 0, 1};
    // mesh_effect.extents = {0, 0, 0};
    // mesh_effect.spawn_rate = 1;
    // mesh_effect.particle_lifetime = 1;
    // mesh_effect.mesh = Asset::Manager::getMesh("Base/Sphere").getResult();
    // mesh_effect.texture = Asset::Manager::getTexture("star_06").getResult();
    // Graphics::getRenderEngine()->getRenderWorld()->addMeshEffect(mesh_effect);

    auto custom_ro = Graphics::CustomRenderObjectCreateData{};
    custom_ro.mesh = Asset::Manager::getMesh("Base/Sphere").getResult();
    custom_ro.pipeline =
        Asset::Manager::getPipeline("Shaders/TestPipeline").getResult();
    custom_ro.model = Matrix::translation(Vector3{0, 0, 0});
    custom_ro.buffer_size = 4;

    handle =
        Graphics::getRenderEngine()->getRenderWorld()->addCustomRenderObject(
            custom_ro);

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
    // player.addScript(
    //     std::make_unique<LookAtScript>(head, player, camera, input));
    player.addScript(std::make_unique<LookAtScript>(player, camera, input));
    // player.addScript(std::make_unique<MoveScript>(player, input));

    Entity stars = world.createEntity();
    stars.addScript(std::make_unique<StarsScript>());
}

void Scene::update(float delta_time) {
    ZoneScoped;

    sky.draw();
    world.update(delta_time);

    auto renderer = Graphics::getRenderEngine();
    auto& render_data = renderer->getRenderWorld()->renderData();
    render_data.time += delta_time;
    render_data.delta_time = delta_time;

    handle.updateData((uint8_t*)&render_data.time, sizeof(delta_time));
}

void Scene::setupSystems() {
    world.addSystem<TransformSystem>();
    world.addSystem<ScriptSystem>();
}

void Scene::parseScene(const File::Scene& scene) {
    auto renderer = Graphics::getRenderEngine();

    for (const auto& effect : scene.effects) {
        auto result = File::Filesystem::getEffect(effect.description);
        auto effect_file = result.getResult();

        auto texture = Asset::Manager::getTexture(effect_file.texture_path);
        if (texture.isError()) return;

        auto effect_description =
            Graphics::EffectDescription{effect.position,
                                        effect_file.extents,
                                        effect_file.spawn_rate,
                                        effect_file.color,
                                        effect_file.size,
                                        effect_file.rotation,
                                        effect_file.particle_lifetime,
                                        texture.getResult()};
        renderer->getRenderWorld()->addEffect(effect_description);
    }

    for (const auto& render_object : scene.render_objects) {
        auto result =
            File::Filesystem::getRenderObject(render_object.description);
        auto render_object_file = result.getResult();

        auto texture =
            Asset::Manager::getTexture(render_object_file.albedo_file);
        if (texture.isError()) return;
        auto mesh = Asset::Manager::getMesh(render_object_file.mesh_file);
        if (mesh.isError()) return;

        auto render_object_data = RenderObjectData{};
        render_object_data.position = render_object.position;
        render_object_data.mesh = mesh.getResult();
        render_object_data.albedo = texture.getResult();
        renderer->getRenderWorld()->addRenderObject(render_object_data);
    }
}
