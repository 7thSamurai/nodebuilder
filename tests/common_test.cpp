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
