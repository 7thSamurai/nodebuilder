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
#include "color.hpp"

TEST(ColorTest, DefaultConstructor) {
    const Color c;

    EXPECT_EQ(c.r, 0x00);
    EXPECT_EQ(c.g, 0x00);
    EXPECT_EQ(c.b, 0x00);
}

TEST(ColorTest, Constructor) {
    const std::uint8_t r = 0x01;
    const std::uint8_t g = 0x02;
    const std::uint8_t b = 0x03;

    const Color c(r, g, b);

    EXPECT_EQ(c.r, r);
    EXPECT_EQ(c.g, g);
    EXPECT_EQ(c.b, b);
}

TEST(ColorTest, Equality) {
    const Color c(0x01, 0x02, 0x03);

    EXPECT_EQ(Color(0x01, 0x02, 0x03) == c, true);
    EXPECT_EQ(Color(0x02, 0x01, 0x03) == c, false);
    EXPECT_EQ(Color(0x01, 0x03, 0x02) == c, false);
    EXPECT_EQ(Color(0x00, 0x00, 0x00) == c, false);
}

TEST(ColorTest, Inequality) {
    const Color c(0x01, 0x02, 0x03);

    EXPECT_EQ(Color(0x01, 0x02, 0x03) != c, false);
    EXPECT_EQ(Color(0x02, 0x01, 0x03) != c, true);
    EXPECT_EQ(Color(0x01, 0x03, 0x02) != c, true);
    EXPECT_EQ(Color(0x00, 0x00, 0x00) != c, true);
}

TEST(ColorTest, FromRgb) {
    EXPECT_EQ(Color::from_rgb(0x01, 0x02, 0x03), Color(0x01, 0x02, 0x03));
}

TEST(ColorTest, FromHsv) {
    EXPECT_EQ(Color::from_hsv(0.0f, 0.0f, 0.0f), Color(0x00, 0x00, 0x00));
    EXPECT_EQ(Color::from_hsv(0.997f, 1.0f, 1.0f), Color(0xff, 0x00, 0x04));
    EXPECT_EQ(Color::from_hsv(0.5f, 1.0f, 1.0f), Color(0x00, 0xff, 0xff));
    EXPECT_EQ(Color::from_hsv(0.0f, 1.0f, 1.0f), Color(0xff, 0x00, 0x00));
}

TEST(ColorTest, Random) {
    std::map<unsigned int, unsigned int> colors;

    for (int i = 0; i < 100; i++) {
        Color color = Color::random();
        unsigned int num = color.r << 16 | color.g << 8 | color.b;

        auto it = colors.find(num);
        if (it == colors.end())
            colors.insert({num, 0});
        else
            it->second++;
    }

    for (const auto &it: colors)
        EXPECT_EQ(it.second > 2, false);
}
