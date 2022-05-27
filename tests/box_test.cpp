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
#include "box.hpp"

TEST(BoxTest, DefaultConstructor) {
    const Boxf b;

    EXPECT_EQ(b.min(), Vec2f(0.0f, 0.0f));
    EXPECT_EQ(b.max(), Vec2f(0.0f, 0.0f));
}

TEST(BoxTest, Constructor) {
    const Vec2f min(1.0f, 2.0f);
    const Vec2f max(3.0f, 4.0f);

    const Boxf b(min, max);

    EXPECT_EQ(b.min(), min);
    EXPECT_EQ(b.max(), max);
}

TEST(BoxTest, Extend) {
    Box b(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f));

    b.extend(Vec2f(0.0f, 0.0f));
    EXPECT_EQ(b, Boxf(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f)));

    b.extend(Vec2f(-10.0f, 0.0f));
    EXPECT_EQ(b, Boxf(Vec2f(-10.0f, -2.0f), Vec2f(3.0f, 4.0f)));

    b.extend(Vec2f(10.0f, 0.0f));
    EXPECT_EQ(b, Boxf(Vec2f(-10.0f, -2.0f), Vec2f(10.0f, 4.0f)));

    b.extend(Vec2f(0.0f, -10.0f));
    EXPECT_EQ(b, Boxf(Vec2f(-10.0f, -10.0f), Vec2f(10.0f, 4.0f)));

    b.extend(Vec2f(0.0f, 10.0f));
    EXPECT_EQ(b, Boxf(Vec2f(-10.0f, -10.0f), Vec2f(10.0f, 10.0f)));
}

TEST(BoxTest, ContainsPoint) {
    const Box b(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(b.contains(Vec2f(0.0f, 0.0f)), true);
    EXPECT_EQ(b.contains(Vec2f(3.0f, 4.0f)), true);
    EXPECT_EQ(b.contains(Vec2f(2.0f, -1.0f)), true);
    EXPECT_EQ(b.contains(Vec2f(-10.0f, 0.0f)), false);
    EXPECT_EQ(b.contains(Vec2f(0.0f, 10.0f)), false);
}

TEST(BoxTest, ClipLine) {
    const Boxf b(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(b.clip(Linef(Vec2f(1.0f, -10.0f), Vec2f(1.0f, 10.0f))), Linef(Vec2f(1.0f, -2.0f), Vec2f(1.0f, 4.0f))); // Vertical
    EXPECT_EQ(b.clip(Linef(Vec2f(-10.0f, 1.0f), Vec2f(10.0f, 1.0f))), Linef(Vec2f(-1.0f, 1.0f), Vec2f(3.0f, 1.0f))); // Horizontal
    EXPECT_EQ(b.clip(Linef(Vec2f(-2.0f, -3.5f), Vec2f(4.0f, 5.5f))), Linef(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f))); // Slopes down
    EXPECT_EQ(b.clip(Linef(Vec2f(-2.0f, 5.5f), Vec2f(4.0, -3.5f))), Linef(Vec2f(-1.0f, 4.0f), Vec2f(3.0, -2.0f)));   // Slopes up

    // Same as above, but reversed order of points
    EXPECT_EQ(b.clip(Linef(Vec2f(1.0f, 10.0f), Vec2f(1.0f, -10.0f))), Linef(Vec2f(1.0f, -2.0f), Vec2f(1.0f, 4.0f))); // Vertical
    EXPECT_EQ(b.clip(Linef(Vec2f(10.0f, 1.0f), Vec2f(-10.0f, 1.0f))), Linef(Vec2f(-1.0f, 1.0f), Vec2f(3.0f, 1.0f))); // Horizontal
    EXPECT_EQ(b.clip(Linef(Vec2f(4.0f, 5.5f), Vec2f(-2.0f, -3.5f))), Linef(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f))); // Slopes down
    EXPECT_EQ(b.clip(Linef(Vec2f(4.0, -3.5f), Vec2f(-2.0f, 5.5f))), Linef(Vec2f(-1.0f, 4.0f), Vec2f(3.0, -2.0f)));   // Slopes up
}

TEST(BoxTest, Corners) {
    const Box b(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(b.top_left(),  Vec2f(-1.0f, -2.0f));
    EXPECT_EQ(b.top_right(), Vec2f(3.0f, -2.0f));
    EXPECT_EQ(b.bot_left(),  Vec2f(-1.0f, 4.0f));
    EXPECT_EQ(b.bot_right(), Vec2f(3.0f, 4.0f));
}

TEST(BoxTest, Size) {
    const Box b(Vec2f(-1.0f, -2.0f), Vec2f(3.0f, 4.0f));

    EXPECT_EQ(b.size(), Vec2f(4.0f, 6.0f));
    EXPECT_EQ(b.width(), 4.0f);
    EXPECT_EQ(b.height(), 6.0f);
}
