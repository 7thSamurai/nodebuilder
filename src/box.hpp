#pragma once

#include "vec.hpp"

template <typename T>
class Box
{
public:
    Box() = default;
    Box(const Vec2<T> &min, const Vec2<T> &max) : min_(min), max_(max) {
    }
    
    /**
     * Extends the bounding-box so that a point fits inside
     * @param p The point to fit inside
     */
    void extend(const Vec2<T> &p) {
        if (p.x < min_.x)
            min_.x = p.x;
        else if (p.y < min_.y)
            min_.y = p.y;
        
        if (p.x > max_.x)
            max_.x = p.x;
        else if (p.y > max_.y)
            max_.y = p.y;
    }

    Vec2<T> min() const { return min; }
    Vec2<T> max() const { return max; }

private:
    Vec2<T> min_, max_;
}; 
