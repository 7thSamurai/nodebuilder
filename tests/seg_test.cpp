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
#include "seg.hpp"

TEST(SegTest, DefaultConstructor) {
    const Seg s;

    EXPECT_EQ(s.line(), Linef(Vec2f(0.0f, 0.0f), Vec2f(0.0f, 0.0f)));
    EXPECT_EQ(s.side(), false);
    EXPECT_EQ(s.offset(), 0.0f);
    EXPECT_EQ(s.linedef(), 0.0f);
}

TEST(SegTest, Constructor) {
    const Linef line(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f));
    const bool side = true;
    const float offset = 100.0f;
    const unsigned int linedef = 200;

    const Seg s(line.a, line.b, side, offset, linedef);

    EXPECT_EQ(s.line(), line);
    EXPECT_EQ(s.side(), side);
    EXPECT_EQ(s.offset(), offset);
    EXPECT_EQ(s.linedef(), linedef);
}

TEST(SegTest, Points) {
    Vec2f p1(1.0f, 2.0f);
    Vec2f p2(3.0f, 4.0f);

    const Seg s(p1, p2, false, 0.0f, 0);

    EXPECT_EQ(s.p1(), p1);
    EXPECT_EQ(s.p2(), p2);
}

TEST(SegTest, Angle) {
    const Vec2f v1(1520.0f, -3168.0f);
    const Vec2f v2(1672.0f, -3104.0f);
    const Vec2f v3(1896.0f, -3104.0f);
    const Vec2f v4(2040.0f, -3144.0f);

    EXPECT_EQ(Seg(v1, v2, false, 0.0f, 0).angle(), 4156.0f);
    EXPECT_EQ(Seg(v2, v3, false, 0.0f, 0).angle(), 0.0f);
    EXPECT_EQ(Seg(v3, v4, false, 0.0f, 0).angle(), -2826.0f);
}
