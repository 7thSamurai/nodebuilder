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
#include "line.hpp"

TEST(LineTest, DefaultConstructor) {
    const Linef l;

    EXPECT_EQ(l.a, Vec2f(0.0f, 0.0f));
    EXPECT_EQ(l.b, Vec2f(0.0f, 0.0f));
}

TEST(LineTest, Constructor) {
    const Vec2f a(1.0f, 2.0f);
    const Vec2f b(3.0f, 4.0f);

    const Linef l(a, b);

    EXPECT_EQ(l.a, a);
    EXPECT_EQ(l.b, b);
}

TEST(LineTest, Equality) {
    const Linef l(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(Linef(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f)) == l, true);
    EXPECT_EQ(Linef(Vec2f(2.0f, 1.0f), Vec2f(3.0f, 4.0f)) == l, false);
    EXPECT_EQ(Linef(Vec2f(1.0f, 2.0f), Vec2f(4.0f, 3.0f)) == l, false);
}

TEST(LineTest, Inequality) {
    const Linef l(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(Linef(Vec2f(1.0f, 2.0f), Vec2f(3.0f, 4.0f)) != l, false);
    EXPECT_EQ(Linef(Vec2f(2.0f, 1.0f), Vec2f(3.0f, 4.0f)) != l, true);
    EXPECT_EQ(Linef(Vec2f(1.0f, 2.0f), Vec2f(4.0f, 3.0f)) != l, true);
}

TEST(LineTest, Math) {
    const Linef l(Vec2f(1.0f, 2.0f), Vec2f(5.0f, 10.0f));

    EXPECT_EQ(l.rise(), 8.0f);
    EXPECT_EQ(l.run(), 4.0f);
    EXPECT_EQ(l.slope(), 2.0f);
}

TEST(LineTest, SideOf) {
    const Linef horiz(Vec2f(-10.0f, 0.0f), Vec2f(10.0f, 0.0f)); // Horizontal
    const Linef vert(Vec2f(0.0f, -10.0f), Vec2f(0.0f, 10.0f));  // Vertical
    const Linef up(Vec2f(-5.0f, -5.0f), Vec2f(5.0f, 5.0f));     // Slopes up
    const Linef down(Vec2f(-5.0f, 5.0f), Vec2f(5.0f, -5.0f));   // Slopes down

    EXPECT_EQ(horiz.side_of(Vec2f(0.0f, -10.0f)), -1);
    EXPECT_EQ(horiz.side_of(Vec2f(0.0f, 10.0f)), 1);
    EXPECT_EQ(horiz.side_of(Vec2f(3.0f, 0.0f)), 0);

    EXPECT_EQ(vert.side_of(Vec2f(-10.0f, 0.0f)), 1);
    EXPECT_EQ(vert.side_of(Vec2f(10.0f, 0.0f)), -1);
    EXPECT_EQ(vert.side_of(Vec2f(0.0f, -4.0f)), 0);

    EXPECT_EQ(up.side_of(Vec2f(-10.0f, 0.0f)), 1);
    EXPECT_EQ(up.side_of(Vec2f(10.0f, 0.0f)), -1);

    EXPECT_EQ(down.side_of(Vec2f(-10.0f, 0.0f)), -1);
    EXPECT_EQ(down.side_of(Vec2f(10.0f, 0.0f)), 1);
}

TEST(LineTest, Intersects) {
    const Linef horiz(Vec2f(-10.0f, 0.0f), Vec2f(10.0f, 0.0f)); // Horizontal
    const Linef vert(Vec2f(0.0f, -10.0f), Vec2f(0.0f, 10.0f));  // Vertical
    const Linef up(Vec2f(-5.0f, -5.0f), Vec2f(5.0f, 5.0f));     // Slopes up
    const Linef down(Vec2f(-5.0f, 5.0f), Vec2f(5.0f, -5.0f));   // Slopes down

    EXPECT_EQ(horiz.intersects(vert), true);
    EXPECT_EQ(horiz.intersects(up), true);
    EXPECT_EQ(horiz.intersects(down), true);

    EXPECT_EQ(vert.intersects(horiz), true);
    EXPECT_EQ(vert.intersects(up), true);
    EXPECT_EQ(vert.intersects(down), true);

    EXPECT_EQ(up.intersects(horiz), true);
    EXPECT_EQ(up.intersects(vert), true);
    EXPECT_EQ(up.intersects(down), true);

    EXPECT_EQ(down.intersects(horiz), true);
    EXPECT_EQ(down.intersects(vert), true);
    EXPECT_EQ(down.intersects(up), true);

    EXPECT_EQ(vert.intersects(vert + Vec2f(-1.0f, 0.0f)), false);
    EXPECT_EQ(vert.intersects(vert + Vec2f(1.0f, 0.0f)), false);
    EXPECT_EQ(vert.intersects(up + Vec2f(-20.0f, 0.0f)), false);
    EXPECT_EQ(vert.intersects(up + Vec2f(20.0f, 0.0f)), false);

    EXPECT_EQ(horiz.intersects(horiz + Vec2f(0.0f, -1.0f)), false);
    EXPECT_EQ(horiz.intersects(horiz + Vec2f(0.0f, 1.0f)), false);
    EXPECT_EQ(horiz.intersects(up + Vec2f(-20.0f, 0.0f)), false);
    EXPECT_EQ(horiz.intersects(up + Vec2f(20.0f, 0.0f)), false);

    EXPECT_EQ(up.intersects(up + Vec2f(-1.0f, 0.0f)), false);
    EXPECT_EQ(up.intersects(up + Vec2f(1.0f, 0.0f)), false);

    EXPECT_EQ(down.intersects(down + Vec2f(-1.0f, 0.0f)), false);
    EXPECT_EQ(down.intersects(down + Vec2f(1.0f, 0.0f)), false);

    EXPECT_EQ(horiz.intersects(vert + Vec2f(-10.0f, 0.0f)), true);
    EXPECT_EQ(horiz.intersects(vert + Vec2f(-11.0f, 0.0f)), false);

    EXPECT_EQ(vert.intersects(horiz + Vec2f(0.0f, -10.0f)), true);
    EXPECT_EQ(vert.intersects(horiz + Vec2f(0.0f, -11.0f)), false);
}

TEST(LineTest, IntersectAt) {
    const Linef horiz(Vec2f(-10.0f, 0.0f), Vec2f(10.0f, 0.0f)); // Horizontal
    const Linef vert(Vec2f(0.0f, -10.0f), Vec2f(0.0f, 10.0f));  // Vertical
    const Linef up(Vec2f(-5.0f, -5.0f), Vec2f(5.0f, 5.0f));     // Slopes up
    const Linef down(Vec2f(-5.0f, 5.0f), Vec2f(5.0f, -5.0f));   // Slopes down

    EXPECT_EQ(horiz.intersect_at(vert), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(horiz.intersect_at(up), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(horiz.intersect_at(down), Vec2f(0.0f, 0.0f));

    EXPECT_EQ(vert.intersect_at(horiz), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(vert.intersect_at(up), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(vert.intersect_at(down), Vec2f(0.0f, 0.0f));

    EXPECT_EQ(up.intersect_at(horiz), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(up.intersect_at(vert), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(up.intersect_at(down), Vec2f(0.0f, 0.0f));

    EXPECT_EQ(down.intersect_at(horiz), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(down.intersect_at(vert), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(down.intersect_at(up), Vec2f(0.0f, 0.0f));

    EXPECT_EQ(horiz.intersect_at(vert + Vec2f(-10.0f, 0.0f)), Vec2f(-10.0f, 0.0f));
    EXPECT_EQ(vert.intersect_at(horiz + Vec2f(0.0f, -10.0f)), Vec2f(0.0f, -10.0f));
}
