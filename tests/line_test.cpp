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
