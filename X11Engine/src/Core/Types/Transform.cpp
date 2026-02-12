#include "Transform.h"

#include <foundation/PxTransform.h>

#include <format>

Transform::Transform() { scale = Vector3(1, 1, 1); }

Transform::Transform(const Vector3& position, const Quaternion& orientation,
                     const Vector3& scale)
    : position(position), orientation(orientation), scale(scale) {}

Transform::Transform(const Vector3& position, const Quaternion& orientation)
    : Transform(position, orientation, Vector3(1, 1, 1)) {}

Transform::Transform(const physx::PxTransform& transform)
    : Transform(transform.p, transform.q) {}

void Transform::setPosition(const Vector3& position) {
    this->position = position;
    is_dirty = true;
}

void Transform::setOrientation(const Quaternion& orientation) {
    this->orientation = orientation;
    is_dirty = true;
}

void Transform::setScale(const Vector3& scale) {
    this->scale = scale;
    is_dirty = true;
}

Vector3 Transform::getPosition() const { return position; }

Quaternion Transform::getOrientation() const { return orientation; }

Vector3 Transform::getScale() const { return scale; }

Vector3 Transform::getForward() const {
    return Vector3(0, 0, 1).rotate(orientation);
}

Vector3 Transform::getRight() const {
    return Vector3(1, 0, 0).rotate(orientation);
}

Vector3 Transform::getUp() const {
    return Vector3(0, 1, 0).rotate(orientation);
}

void Transform::markDirty() { is_dirty = true; }

void Transform::markClean() { is_dirty = false; }

bool Transform::isDirty() const { return is_dirty; }

std::string Transform::serialize() const {
    auto position_str = std::format("Position: ({}, {}, {})", position.x,
                                    position.y, position.z);
    auto orientation_str =
        std::format("Orientation: ({}, {}, {}, {})", orientation.w,
                    orientation.x, orientation.y, orientation.z);
    auto scale_str =
        std::format("Scale: ({}, {}, {})", scale.x, scale.y, scale.z);

    return std::format("{}\n{}\n{}", position_str, orientation_str, scale_str);
}

Transform::operator physx::PxTransform() const {
    return physx::PxTransform(position, orientation);
}
