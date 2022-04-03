#pragma once

#include "vec.hpp"
#include "common.hpp"
#include <cmath>

// TODO: Linedef

class Seg
{
public:
    Seg() : side_(false), offset_(0.0f) {
    }
    
    Seg(const Vec2f &p1, const Vec2f &p2, bool side, float offset) : p1_(p1), p2_(p2), side_(side), offset_(offset) {
    }

    inline Vec2f p1() const { return p1_; }
    inline Vec2f p2() const { return p2_; }
    
    inline bool side() const { return side_; }
    inline float offset() const { return offset_; }

    // Binary Angle Measurement 
    std::uint16_t angle() const {
        float dx = p2_.x - p1_.x;
        float dy = p2_.y - p1_.y;
        
        // degrees = radians * 180 / PI
        // (0x8000 = 180 BAM)
        return std::atan2(dy, dx) * 0x8000 / Common::PI;
    }

private:
    Vec2f p1_, p2_; // Start and end points
    bool side_;     // Side/Direction
    float offset_;  // Offset along linedef to start of seg
}; 
