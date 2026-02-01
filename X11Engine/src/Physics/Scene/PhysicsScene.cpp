#include "PhysicsScene.h"

#include <PxPhysicsAPI.h>

#include "Actor.h"
#include "Resources.h"

namespace Physics {

Scene::Scene() {
    auto physics = Resources::getPhysics();

    dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

    physx::PxSceneDesc desc(physics->getTolerancesScale());
    desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
    desc.cpuDispatcher = dispatcher;
    desc.filterShader = physx::PxDefaultSimulationFilterShader;
    scene = physics->createScene(desc);
}

Scene::~Scene() { dispatcher->release(); }

void Scene::addActor(physx::PxActor* actor) {
    bool result = scene->addActor(*actor);
}

void Scene::simulate(float delta_time) {
    scene->simulate(delta_time);
    scene->fetchResults(true);
}

};  // namespace Physics