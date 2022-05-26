#include <gtest/gtest.h>
#include "polygon.hpp"

TEST(PolyTest, Size) {
    Polyf poly;

    EXPECT_EQ(poly.size(), 0);

    poly.add(Vec2f(0.0f, 0.0f));
    poly.add(Vec2f(0.0f, 0.0f));
    poly.add(Vec2f(0.0f, 0.0f));

    EXPECT_EQ(poly.size(), 3);
}

TEST(PolyTest, At) {
    Polyf poly;

    poly.add(Vec2f(1.0f, 1.0f));
    poly.add(Vec2f(2.0f, 2.0f));
    poly.add(Vec2f(3.0f, 3.0f));

    EXPECT_EQ(poly.at(0), Vec2f(1.0f, 1.0f));
    EXPECT_EQ(poly.at(1), Vec2f(2.0f, 2.0f));
    EXPECT_EQ(poly.at(2), Vec2f(3.0f, 3.0f));
    EXPECT_EQ(poly.at(3), Vec2f(1.0f, 1.0f));
    EXPECT_EQ(poly.at(4), Vec2f(2.0f, 2.0f));
}

TEST(PolyTest, PointInside) {
    Polyf poly;

    poly.add(Vec2f(5.0f, 0.0f));
    poly.add(Vec2f(0.0f, 10.0f));
    poly.add(Vec2f(10.0f, 10.0f));

    EXPECT_EQ(poly.point_inside(Vec2f(0.0f, 0.0f)), false);
    EXPECT_EQ(poly.point_inside(Vec2f(10.0f, 0.0f)), false);
    EXPECT_EQ(poly.point_inside(Vec2f(0.0f, 10.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(10.0f, 10.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(5.0f, 0.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(5.0f, 5.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(5.0f, 10.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(5.0f, 11.0f)), false);
    EXPECT_EQ(poly.point_inside(Vec2f(2.4f, 5.0f)), false);
    EXPECT_EQ(poly.point_inside(Vec2f(2.5f, 5.0f)), true);
    EXPECT_EQ(poly.point_inside(Vec2f(2.6f, 5.0f)), true);
}

TEST(PolyTest, Bounds) {
    Polyf poly;

    poly.add(Vec2f(5.0f, 0.0f));
    poly.add(Vec2f(0.0f, 10.0f));
    poly.add(Vec2f(10.0f, 10.0f));

    EXPECT_EQ(poly.bounds(), Boxf(Vec2f(0.0f, 0.0f), Vec2f(10.0f, 10.0f)));
}
