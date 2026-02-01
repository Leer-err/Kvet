#include "Quaternion.h"

#include <DirectXMath.h>
#include <foundation/PxTransform.h>

#include "Vector3.h"

Quaternion::Quaternion() {
    DirectX::XMStoreFloat4(&quat, DirectX::XMQuaternionIdentity());
}

Quaternion::Quaternion(const Vector3& axis, float angle) {
    auto axis_loaded = DirectX::XMLoadFloat3(&axis.vec);
    auto result_loaded = DirectX::XMQuaternionRotationAxis(axis_loaded, angle);

    DirectX::XMStoreFloat4(&this->quat, result_loaded);
}

Quaternion::Quaternion(float pitch, float yaw, float roll) {
    DirectX::XMStoreFloat4(
        &quat, DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

Quaternion::Quaternion(const physx::PxQuat& vec)
    : Quaternion(vec.w, vec.x, vec.y, vec.z) {}

Quaternion::Quaternion(float w, float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion::Quaternion(const Vector3& vec) : Quaternion(vec.x, vec.y, vec.z) {}

Quaternion Quaternion::slerp(const Quaternion& a, const Quaternion& b,
                             float factor) {
    DirectX::XMVECTOR a_loaded = DirectX::XMLoadFloat4(&a.quat);
    DirectX::XMVECTOR b_loaded = DirectX::XMLoadFloat4(&b.quat);

    DirectX::XMVECTOR result_loaded =
        DirectX::XMQuaternionSlerp(a_loaded, b_loaded, factor);

    DirectX::XMFLOAT4 result;
    DirectX::XMStoreFloat4(&result, result_loaded);
    return Quaternion(result.w, result.x, result.y, result.z);
}

Quaternion Quaternion::inverse() const {
    DirectX::XMVECTOR loaded = DirectX::XMLoadFloat4(&quat);

    DirectX::XMVECTOR result_loaded = DirectX::XMQuaternionInverse(loaded);

    DirectX::XMFLOAT4 result;
    DirectX::XMStoreFloat4(&result, result_loaded);
    return Quaternion(result.w, result.x, result.y, result.z);
}

bool Quaternion::operator==(const Quaternion& b) const {
    DirectX::XMVECTOR a_loaded = DirectX::XMLoadFloat4(&quat);
    DirectX::XMVECTOR b_loaded = DirectX::XMLoadFloat4(&b.quat);
    return DirectX::XMQuaternionEqual(a_loaded, b_loaded);
}

Quaternion operator*(const Quaternion& a, const Quaternion& b) {
    DirectX::XMVECTOR a_loaded = DirectX::XMLoadFloat4(&a.quat);
    DirectX::XMVECTOR b_loaded = DirectX::XMLoadFloat4(&b.quat);
    Quaternion quat;

    DirectX::XMStoreFloat4(&quat.quat,
                           DirectX::XMQuaternionMultiply(a_loaded, b_loaded));

    return quat;
}

Quaternion::operator physx::PxQuat() const { return physx::PxQuat(x, y, z, w); }