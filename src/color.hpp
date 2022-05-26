#pragma once

#include <cstdint>

class Color
{
public:
    Color() : r(0), g(0), b(0) {
    }

    Color(std::uint8_t r0, std::uint8_t g0, std::uint8_t b0) : r(r0), g(g0), b(b0) {
    }

    bool operator == (const Color &c) const {
        return r == c.r && g == c.g && b == c.b;
    }

    bool operator != (const Color &c) const {
        return r != c.r || g != c.g || b != c.b;
    }

    static Color from_rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
        return Color(r, g, b);
    }

    static Color from_hsv(float h, float s, float v) {
        int h_i = h * 6.0f;
        float fract = h*6.0f - h_i;

        float p = v * (1.0f - s);
        float q = v * (1.0f - s * fract);
        float t = v * (1.0f - s * (1.0f - fract));

        if (h_i == 0)
            return Color(v*255, t*255, p*255);
        else if (h_i == 1)
            return Color(q*255, v*255, p*255);
        else if (h_i == 2)
            return Color(p*255, v*255, t*255);
        else if (h_i == 3)
            return Color(p*255, q*255, v*255);
        else if (h_i == 4)
            return Color(t*255, p*255, v*255);
        else if (h_i == 5)
            return Color(v*255, p*255, q*255);

        return Color(0.0f, 0.0f, 0.0f);
    }

    static Color random() {
        float golden_ratio_conjugate = 0.618033988749895;
        float h = static_cast<float>(rand()) / RAND_MAX;
        h += golden_ratio_conjugate;
        if (h > 1.0f) h -= 1.0f;

        return from_hsv(h, 0.85f, 0.99f);
    }

    std::uint8_t r, g, b;
};
