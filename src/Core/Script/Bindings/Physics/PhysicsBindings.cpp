#include "PhysicsBindings.h"

#include "DynamicRigidBody.h"
#include "Resources.h"
#include "Shape.h"
#include "StaticRigidBody.h"

namespace Script::Bindings {

void initShapeType() {
    using Physics::Shape;

    auto state = Resources::get().getState();

    auto shape_type = state.new_usertype<Shape>("Shape", sol::no_constructor);
    shape_type["sphere"] = sol::factories(&Shape::sphere);
    shape_type["box"] = sol::factories(&Shape::box);
    shape_type["capsule"] = sol::factories(&Shape::capsule);
    shape_type["plane"] = sol::factories(&Shape::plane);
    shape_type["setTrigger"] = &Shape::setTrigger;
    shape_type["setPhysical"] = &Shape::setPhysical;
    shape_type["setTransform"] = &Shape::setTransform;
}

void initDynamicRigidBodyType() {
    using Physics::DynamicRigidBody;

    auto state = Resources::get().getState();

    auto locks = state.new_usertype<DynamicRigidBody::Lock>("Lock");
    locks["LockRotationX"] = DynamicRigidBody::Lock::LockRotationX;
    locks["LockRotationY"] = DynamicRigidBody::Lock::LockRotationY;
    locks["LockRotationZ"] = DynamicRigidBody::Lock::LockRotationZ;
    locks["LockRotationXY"] = DynamicRigidBody::Lock::LockRotationXY;
    locks["LockRotationXZ"] = DynamicRigidBody::Lock::LockRotationXZ;
    locks["LockRotationYZ"] = DynamicRigidBody::Lock::LockRotationYZ;
    locks["LockRotationXYZ"] = DynamicRigidBody::Lock::LockRotationXYZ;

    auto type = state.new_usertype<DynamicRigidBody>(
        "DynamicRigidBody",
        sol::constructors<DynamicRigidBody(),
                          DynamicRigidBody(const Transform&)>);
    type["getPosition"] = &DynamicRigidBody::getPosition;
    type["getRotation"] = &DynamicRigidBody::getRotation;
    type["setTransform"] = &DynamicRigidBody::setTransform;
    type["enableGravity"] = &DynamicRigidBody::enableGravity;
    type["lock"] = &DynamicRigidBody::lock;
    type["addShape"] = &DynamicRigidBody::addShape;
    type["velocity"] = sol::property(&DynamicRigidBody::getVelocity,
                                     &DynamicRigidBody::setVelocity);
}

void initStaticRigidBodyType() {
    using Physics::StaticRigidBody;

    auto state = Resources::get().getState();

    auto type = state.new_usertype<StaticRigidBody>(
        "StaticRigidBody",
        sol::constructors<StaticRigidBody(),
                          StaticRigidBody(const Transform&)>);
    type["setTransform"] = sol::overload(
        static_cast<void (StaticRigidBody::*)(
            const Vector3&, const Quaternion&)>(&StaticRigidBody::setTransform),
        static_cast<void (StaticRigidBody::*)(cconst Transform&)>(
            &StaticRigidBody::setTransform));
    type["setTransform"] = &StaticRigidBody::setTransform;
    type["addShape"] = &StaticRigidBody::addShape;
}

void initPhysicsBindings() {
    initShapeType();
    initStaticRigidBodyType();
    initDynamicRigidBodyType();
}

}  // namespace Script::Bindings