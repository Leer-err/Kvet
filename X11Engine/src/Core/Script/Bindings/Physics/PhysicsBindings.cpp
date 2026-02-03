#include "PhysicsBindings.h"

#include "Resources.h"
#include "Shape.h"

using Physics::Shape;

namespace Script::Bindings {

void initPhysicsBindings() {
    auto state = Resources::get().getState();

    auto shape_type = state.new_usertype<Physics::Shape>("Shape");
    shape_type["sphere"] = sol::factories(&Shape::sphere);
    shape_type["box"] = sol::factories(&Shape::box);
    shape_type["capsule"] = sol::factories(&Shape::capsule);
    shape_type["plane"] = sol::factories(&Shape::plane);
    shape_type["setTrigger"] = &Shape::setTrigger;
    shape_type["setPhysical"] = &Shape::setPhysical;
    shape_type["setTransform"] = &Shape::setTransform;
}

}  // namespace Script::Bindings