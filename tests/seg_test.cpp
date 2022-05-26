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
