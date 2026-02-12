#include "Quaternion.h"

#include <foundation/PxTransform.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Vector3.h"

using namespace glm;

static const quat& get(const Quaternion& vec) {
    return reinterpret_cast<const quat&>(vec);
}
static quat& get(Quaternion& vec) { return reinterpret_cast<quat&>(vec); }

Quaternion::Quaternion() : Quaternion(1, 0, 0, 0) {}

Quaternion::Quaternion(const Vector3& axis, float angle) {
    get(*this) = quat(angle, reinterpret_cast<const vec3&>(axis));
}

Quaternion::Quaternion(float pitch, float yaw, float roll)
    : Quaternion(Vector3{pitch, yaw, roll}) {}

Quaternion::Quaternion(const physx::PxQuat& vec)
    : Quaternion(vec.w, vec.x, vec.y, vec.z) {}

Quaternion::Quaternion(float w, float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion::Quaternion(const Vector3& angles) {
    get(*this) = quat(reinterpret_cast<const vec3&>(angles));
}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b,
                             float factor) {
    Quaternion result;
    get(result) = mix(get(a), get(b), factor);

    return result;
}

Quaternion Quaternion::inverse() const {
    Quaternion result;
    get(result) = glm::inverse(get(*this));

    return result;
}

bool Quaternion::operator==(const Quaternion& b) const {
    return get(*this) == get(b);
}

Quaternion operator*(const Quaternion& a, const Quaternion& b) {
    Quaternion result;
    get(result) = get(a) * get(b);

    return result;
}

Quaternion::operator physx::PxQuat() const { return physx::PxQuat(x, y, z, w); }