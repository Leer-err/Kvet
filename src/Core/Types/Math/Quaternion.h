#ifndef QUATERNION_H
#define QUATERNION_H

namespace physx {

template <typename T>
class PxQuatT;
typedef PxQuatT<float> PxQuat;

};  // namespace physx

struct Vector3;

struct Quaternion {
    union {
        struct {
            float x, y, z, w;
        };
    };

    Quaternion();
    Quaternion(const Vector3& axis, float angle);
    Quaternion(float pitch, float yaw, float roll);
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Vector3& vec);
    Quaternion(const physx::PxQuat& vec);

    static Quaternion slerp(const Quaternion& a, const Quaternion& b,
                            float factor);
    Quaternion inverse() const;

    bool operator==(const Quaternion& b) const;

    operator physx::PxQuat() const;
};

Quaternion operator*(const Quaternion& a, const Quaternion& b);

#endif