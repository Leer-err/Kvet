#include "Scene.h"

#include <memory>
#include <tracy/Tracy.hpp>

#include "CameraManager.h"
// #include "DynamicRigidBody.h"
// #include "Name.h"
// #include "PhysicsSystem.h"
// #include "PlayerLook.h"
#include "GraphicsCommunicationManager.h"
#include "Quaternion.h"
// #include "ScriptSystem.h"
// #include "Shape.h"
// #include "StaticMeshRenderSystem.h"
// #include "StaticProjectionCamera.h"
// #include "StaticRigidBody.h"
#include "Transform.h"
// #include "TransformSystem.h"
#include "Vector3.h"

Scene::Scene() {
    // auto cam = std::make_shared<StaticProjectionCamera>(60.f, (float)4 / 3,
    //                                                     0.1f, 1000.f);
    // CameraManager::get().setMainCamera(cam);

    // setupSystems();

    // player = world.createEntity();
    // player.set<Name>({"Player"});
    // player.set<Transform>({});
    // player.addScript(
    //     std::move(std::make_unique<Scripts::Player::PlayerLookX>()));

    // Entity camera = world.createEntity();
    // camera.set<Name>({"Camera"});
    // camera.set<Transform>({});
    // camera.addScript(
    //     std::move(std::make_unique<Scripts::Player::CameraLookY>()));
    // CameraManager::get().setMainCameraEntity(camera);

    // player.addChild(camera);

    // auto ground = world.createEntity();
    // ground.set<Name>({"Ground"});

    // auto ground_shape = Physics::Shape::plane();
    // ground_shape.setTransform(
    //     Transform(Vector3(), Quaternion(Vector3(0, 0, 1), 1.57)));
    // auto ground_actor = Physics::StaticRigidBody(Transform());
    // ground_actor.addShape(ground_shape);
    // ground.set(ground_actor);

    // auto projectile = world.createEntity();
    // projectile.set<Name>({"Projectile"});

    // auto projectile_transform = Transform();
    // auto projectile_shape = Physics::Shape::sphere(1);
    // projectile_shape.setTransform(projectile_transform);
    // auto projectile_actor =
    //     Physics::DynamicRigidBody(Transform(Vector3(0, 1, 0), Quaternion()));
    // projectile_actor.addShape(projectile_shape);
    // projectile_actor.setGravity(true);
    // projectile_actor.setVelocity(Vector3(0.5, 0, 0));
    // projectile.set(std::move(projectile_actor));
}

void Scene::update(float deltaTime) {
    ZoneScoped;

    sky.draw();
    world.update(deltaTime);

    // CameraManager::get().updateCameraData();
    GraphicsCommunicationManager::get().send(
        CameraManager::get().getCameraData());
}

void Scene::setupSystems() {
    // world.add<ScriptSystem>();
    // world.add<TransformSystem>();
    // world.add<StaticMeshRenderSystem>();
    // world.add<PhysicsSystem>(0.01f);
}

World& Scene::getWorld() { return world; }
