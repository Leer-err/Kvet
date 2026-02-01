#ifndef STATIC_RIGID_BODY_H
#define STATIC_RIGID_BODY_H

#include "Actor.h"
#include "Quaternion.h"
#include "Transform.h"

namespace physx {
class PxRigidStatic;
class PxActor;
}  // namespace physx

namespace Physics {

class StaticRigidBody : public Actor {
   public:
    StaticRigidBody();
    StaticRigidBody(const Transform& transform);
    ~StaticRigidBody();

    void setTransform(const Vector3& position, const Quaternion& orientation);
    void setTransform(const Transform& transform);

    void addShape(const Shape& shape) override;

    physx::PxRigidStatic* get() const;

   private:
    physx::PxRigidStatic* body;
};

};  // namespace Physics

#endif  // STATIC_RIGID_BODY_H