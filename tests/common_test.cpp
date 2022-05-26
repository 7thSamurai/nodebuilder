#include <gtest/gtest.h>
#include "common.hpp"

TEST(CommonTest, Swap) {
    EXPECT_EQ(Common::swap16(0x1234), 0x3412);
    EXPECT_EQ(Common::swap32(0x12345678), 0x78563412);
}

TEST(CommonTest, Sign) {
    EXPECT_EQ(Common::sign(0), 0);
    EXPECT_EQ(Common::sign(100), 1);
    EXPECT_EQ(Common::sign(-100), -1);
}
