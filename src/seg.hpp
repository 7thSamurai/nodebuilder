#pragma once

#include "vec.hpp"
#include "line.hpp"
#include "common.hpp"
#include <cmath>

class Seg
{
public:
    Seg() : side_(false), offset_(0.0f), linedef_(0) {
    }

    Seg(const Vec2f &p1, const Vec2f &p2, bool side, float offset, unsigned int linedef) : line_(p1, p2), side_(side), offset_(offset), linedef_(linedef) {
    }

    inline Vec2f p1() const { return line_.a; }
    inline Vec2f p2() const { return line_.b; }
    inline Linef line() const { return line_; }

    inline bool side() const { return side_; }
    inline float offset() const { return offset_; }
    inline unsigned int linedef() const { return linedef_; }

    // Binary Angle Measurement
    std::uint16_t angle() const {
        float dx = line_.b.x - line_.a.x;
        float dy = line_.b.y - line_.a.y;

        // degrees = radians * 180 / PI
        // (0x8000 = 180 BAM)
        return std::atan2(dy, dx) * 0x8000 / Common::PI;
    }

private:
    Linef line_;
    bool side_;     // Side/Direction
    float offset_;  // Offset along linedef to start of seg
    unsigned int linedef_;
};
