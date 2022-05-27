// Copyright (C) 2022 Zach Collins <zcollins4@proton.me>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
