#ifndef STATIC_RIGID_BODY_H
#define STATIC_RIGID_BODY_H

#include "Quaternion.h"
#include "Transform.h"

namespace physx {
class PxRigidStatic;
}  // namespace physx

namespace Physics {

class StaticRigidBody {
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