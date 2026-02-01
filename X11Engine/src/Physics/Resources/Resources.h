#pragma once

namespace physx {
class PxFoundation;
class PxPhysics;
class PxPvd;
}  // namespace physx

namespace Physics {

class Resources {
   public:
    static physx::PxPhysics* getPhysics();

   private:
    Resources();

    static Resources& get();

    physx::PxFoundation* foundation;
    physx::PxPhysics* physics;
    physx::PxPvd* pvd;
};

};  // namespace Physics