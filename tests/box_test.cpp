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
