#include "Shape.h"

#include <PxMaterial.h>
#include <PxPhysics.h>
#include <PxShape.h>
#include <geometry/PxPlaneGeometry.h>

#include "Resources.h"

namespace Physics {

Shape Shape::sphere(float radius) {
    auto geometry = physx::PxSphereGeometry(radius);
    return Shape(geometry);
}

Shape Shape::box(const Vector3& half_extents) {
    auto geometry =
        physx::PxBoxGeometry(half_extents.x, half_extents.y, half_extents.z);
    return Shape(geometry);
}

Shape Shape::capsule(float radius, float half_height) {
    auto geometry = physx::PxCapsuleGeometry(radius, half_height);
    return Shape(geometry);
}

Shape Shape::plane() {
    auto geometry = physx::PxPlaneGeometry();
    return Shape(geometry);
}

Shape::Shape() : shape(nullptr) {}

Shape::Shape(const physx::PxGeometry& geometry) {
    auto physics = Resources::getPhysics();

    auto material = physics->createMaterial(0.5f, 0.5f, 0.1f);
    shape = physics->createShape(geometry, *material);

    material->release();
}

Shape::Shape(const Shape& shape) {
    this->shape = shape.shape;

    this->shape->acquireReference();
}

Shape::~Shape() {
    if (shape == nullptr) return;
    shape->release();
}

void Shape::setTransform(const Transform& transform) {
    shape->setLocalPose(transform);
}

void Shape::isPhysical() {
    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
}

void Shape::isTrigger() {
    shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
}

physx::PxShape* Shape::get() const { return shape; }

};  // namespace Physics