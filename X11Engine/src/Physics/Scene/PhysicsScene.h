#pragma once

#include "extensions/PxDefaultCpuDispatcher.h"
namespace physx {
class PxScene;
class PxActor;
}  // namespace physx

namespace Physics {

class Scene {
    friend class StaticRigidBody;
    friend class DynamicRigidBody;

   public:
    static Scene& get() {
        static Scene instance;
        return instance;
    }

    void simulate(float delta_time);

   protected:
    void addActor(physx::PxActor* actor);

   private:
    Scene();
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    physx::PxScene* scene;
    physx::PxDefaultCpuDispatcher* dispatcher;
};

};  // namespace Physics