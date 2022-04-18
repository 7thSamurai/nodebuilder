#pragma once

#include <cstdint>

class Color
{
public:
    Color() : r(0), g(0), b(0) {
    }

    Color(std::uint8_t r0, std::uint8_t g0, std::uint8_t b0) : r(r0), g(g0), b(b0) {
    }

    static Color random() {
        int r = (128 + (rand() % 256)) / 2;
        int g = (128 + (rand() % 256)) / 2;
        int b = (128 + (rand() % 256)) / 2;

        return Color(r, g, b);
    }

    std::uint8_t r, g, b;
};
