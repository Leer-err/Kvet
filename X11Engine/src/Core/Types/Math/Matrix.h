#ifndef MATRIX_H
#define MATRIX_H

#include <cstring>

#include "Quaternion.h"
#include "Vector3.h"

struct Matrix {
    float m[4][4];

    static Matrix identity();

    static Matrix projection(float fov, float aspect_ratio, float far,
                             float near);

    static Matrix translation(const Vector3& pos);
    static Matrix rotation(float roll, float pitch, float yaw);
    static Matrix rotation(const Quaternion& q);
    static Matrix scale(const Vector3& scale);
    static Matrix view(const Vector3& position, const Vector3& forward,
                       const Vector3& up);

    Matrix transpose() const;

    Matrix inverse() const;

    Vector3 getTranslation() const;
    Quaternion getRotation() const;
    Vector3 getScale() const;

    void decompose(Vector3& position, Vector3& scale,
                   Quaternion& orientation) const;
};

Matrix operator*(const Matrix& a, const Matrix& b);

inline bool operator==(const Matrix& a, const Matrix& b) {
    return memcmp(&a, &b, sizeof(Matrix)) == 0;
}

#endif  // MATRIX_H