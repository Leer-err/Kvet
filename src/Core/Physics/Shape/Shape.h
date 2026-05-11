#ifndef SHAPE_H
#define SHAPE_H

#include "Transform.h"
#include "Vector3.h"

namespace physx {
class PxShape;
class PxGeometry;
};  // namespace physx

namespace Physics {

class Shape {
    friend class StaticRigidBody;
    friend class DynamicRigidBody;

   public:
    Shape();
    Shape(const Shape& shape);
    ~Shape();

    static Shape sphere(float radius);
    static Shape box(const Vector3& half_extents);
    static Shape capsule(float radius, float half_height);
    static Shape plane();

    void setTrigger();
    void setPhysical();

    void setTransform(const Transform& transform);

   protected:
    physx::PxShape* get() const;

   private:
    Shape(const physx::PxGeometry& geometry);

    physx::PxShape* shape;
};

};  // namespace Physics

#endif  // SHAPE_H