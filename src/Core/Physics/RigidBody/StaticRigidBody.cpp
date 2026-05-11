#include "StaticRigidBody.h"

#include <PxActor.h>
#include <PxPhysics.h>
#include <PxRigidStatic.h>
#include <foundation/PxQuat.h>
#include <foundation/PxTransform.h>

#include "PhysicsScene.h"
#include "Resources.h"
#include "Transform.h"
#include "Vector3.h"

namespace Physics {

StaticRigidBody::StaticRigidBody() : body(nullptr) {}

StaticRigidBody::StaticRigidBody(const Transform& transform) {
    auto physics = Resources::getPhysics();

    body = physics->createRigidStatic(transform);
    Scene::get().addActor(body);
}

StaticRigidBody::~StaticRigidBody() {
    if (body == nullptr) return;

    // body->release();
}

void StaticRigidBody::setTransform(const Transform& transform) {
    body->setGlobalPose(transform);
}

void StaticRigidBody::setTransform(const Vector3& position,
                                   const Quaternion& orientation) {
    setTransform(Transform(position, orientation));
}

void StaticRigidBody::addShape(const Shape& shape) {
    bool success = body->attachShape(*shape.get());
}

physx::PxRigidStatic* StaticRigidBody::get() const { return body; }

};  // namespace Physics