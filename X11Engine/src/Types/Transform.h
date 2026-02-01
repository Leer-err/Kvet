#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <string>

#include "Quaternion.h"
#include "Vector3.h"

namespace physx {
template <typename T>
class PxTransformT;

typedef PxTransformT<float> PxTransform;
}  // namespace physx

class Transform {
   public:
    Transform();

    Transform(const Vector3& position, const Quaternion& orientation,
              const Vector3& scale);
    Transform(const Vector3& position, const Quaternion& orientation);
    Transform(const physx::PxTransform& transform);

    void setPosition(const Vector3& position);
    void setOrientation(const Quaternion& orientation);
    void setScale(const Vector3& scale);

    Vector3 getPosition() const;
    Quaternion getOrientation() const;
    Vector3 getScale() const;

    Vector3 getForward() const;
    Vector3 getRight() const;
    Vector3 getUp() const;

    void markClean();
    void markDirty();

    bool isDirty() const;

    std::string serialize() const;

    operator physx::PxTransform() const;

   private:
    Vector3 position;
    Quaternion orientation;
    Vector3 scale;

    bool is_dirty;
};

#endif  // TRANSFORM_H