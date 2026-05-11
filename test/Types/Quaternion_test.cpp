#include "Quaternion.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(QuaternionTest, FromEuler0) {
    Quaternion identity(1, 0, 0, 0);

    ASSERT_EQ(Quaternion(), identity);
}

TEST(QuaternionTest, FromRoll) {
    Quaternion roll(0., 1, 0, 0);

    ASSERT_EQ(Quaternion(3.14f, 0.f, 0.f), roll);
}

TEST(QuaternionTest, ToMatrix) {
    // Quaternion quat;
    // quat.w = 0.;
    // quat.x = 1.;
    // quat.y = 0.;
    // quat.z = 0.;

    // Matrix rotation = quat.toRotationMatrix();

    // Matrix expected = Matrix::identity();

    // expected.matrix[1][1] = -1;
    // expected.matrix[2][2] = -1;

    // ASSERT_EQ(rotation, expected);
}