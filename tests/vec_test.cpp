#include <gtest/gtest.h>
#include "vec.hpp"

TEST(VectorTest, DefaultConstructor) {
    const Vec2f v;

    EXPECT_EQ(v.x, 0.0f);
    EXPECT_EQ(v.y, 0.0f);
}

TEST(VectorTest, Constructor) {
    const float x = 1.0f;
    const float y = 2.0f;

    const Vec2f v(x, y);

    EXPECT_EQ(v.x, x);
    EXPECT_EQ(v.y, y);
}

TEST(VectorTest, Equality) {
    const Vec2f v(1.0f, 2.0f);

    EXPECT_EQ(Vec2f(1.0f, 2.0f) == v, true);
    EXPECT_EQ(Vec2f(2.0f, 1.0f) == v, false);
    EXPECT_EQ(Vec2f(0.0f, 0.0f) == v, false);
}

TEST(VectorTest, Inequality) {
    const Vec2f v(1.0f, 2.0f);

    EXPECT_EQ(Vec2f(1.0f, 2.0f) != v, false);
    EXPECT_EQ(Vec2f(2.0f, 1.0f) != v, true);
    EXPECT_EQ(Vec2f(0.0f, 0.0f) != v, true);
}

TEST(VectorTest, VectorMath) {
    const Vec2f v1(30.0f, 20.0f);
    const Vec2f v2(10.0f, 5.0f);

    EXPECT_EQ(v1 + v2, Vec2f(40.0f, 25.0f));
    EXPECT_EQ(v1 - v2, Vec2f(20.0f, 15.0f));
    EXPECT_EQ(v1 * v2, Vec2f(300.0f, 100.0f));
    EXPECT_EQ(v1 / v2, Vec2f(3.0f, 4.0f));
}

TEST(VectorTest, FloatMath) {
    const Vec2f v1(20.0f, 40.0f);
    const float f = 10.0f;

    EXPECT_EQ(v1 + f, Vec2f(30.0f, 50.0f));
    EXPECT_EQ(v1 - f, Vec2f(10.0f, 30.0f));
    EXPECT_EQ(v1 * f, Vec2f(200.0f, 400.0f));
    EXPECT_EQ(v1 / f, Vec2f(2.0f, 4.0f));
}
