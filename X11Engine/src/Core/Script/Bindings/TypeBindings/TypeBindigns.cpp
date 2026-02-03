#include "Quaternion.h"
#include "Resources.h"
#include "TypeBindings.h"
#include "Vector3.h"

namespace Script::Bindings {

void initTypeBindings() {
    auto state = Resources::get().getState();

    auto vector3_type = state.new_usertype<Vector3>(
        "Vector3", sol::constructors<Vector3(), Vector3(float, float, float)>(),
        sol::meta_function::addition,
        sol::resolve<Vector3(const Vector3&, const Vector3&)>(::operator+),
        sol::meta_function::subtraction,
        sol::resolve<Vector3(const Vector3&, const Vector3&)>(::operator-),
        sol::meta_function::multiplication,
        sol::resolve<Vector3(const Vector3&, float)>(::operator*),
        sol::meta_function::division,
        sol::resolve<Vector3(const Vector3&, float)>(::operator/));
    vector3_type["lerp"] = sol::factories(&Vector3::lerp);
    vector3_type["normalized"] = &Vector3::normalized;
    vector3_type["length"] = &Vector3::length;
    vector3_type["rotate"] = &Vector3::rotate;
    vector3_type["x"] = &Vector3::x;
    vector3_type["y"] = &Vector3::y;
    vector3_type["z"] = &Vector3::z;

    auto quaternion_type = state.new_usertype<Quaternion>(
        "Quaternion",
        sol::constructors<Quaternion(), Quaternion(float, float, float),
                          Quaternion(float, float, float, float),
                          Quaternion(const Vector3&),
                          Quaternion(const Vector3&, float)>(),
        sol::meta_function::equal_to, &Quaternion::operator*,
        sol::meta_function::multiplication,
        sol::resolve<Quaternion(const Quaternion&, const Quaternion&)>(
            ::operator*));
    quaternion_type["slerp"] = sol::factories(&Quaternion::slerp);
    quaternion_type["inverse"] = &Quaternion::inverse;
    quaternion_type["x"] = &Quaternion::x;
    quaternion_type["y"] = &Quaternion::y;
    quaternion_type["z"] = &Quaternion::z;
    quaternion_type["w"] = &Quaternion::w;

    auto transform_type = state.new_usertype<Transform>(
        "Transform",
        sol::constructors<Transform(),
                          Transform(const Vector3&, const Quaternion&,
                                    const Vector3&),
                          Transform(const Vector3&, const Quaternion&)>());
    quaternion_type["position"] =
        sol::property(&Transform::getPosition, &Transform::setPosition);
    quaternion_type["orientation"] =
        sol::property(&Transform::getOrientation, &Transform::setOrientation);
    quaternion_type["scale"] =
        sol::property(&Transform::getScale, &Transform::setScale);
}

}  // namespace Script::Bindings