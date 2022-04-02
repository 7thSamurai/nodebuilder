#pragma once

template <typename T>
class Vec2
{
public:
    Vec2() {
        x = y = 0;
    }
    
    Vec2(const T &x, const T &y) {
        this->x = x;
        this->y = y;
    }
    
    bool operator == (const Vec2 &p) const {
        return x == p.x && y == p.y;
    }
    
    bool operator != (const Vec2 &p) const {
        return x != p.x || y != p.y;
    }

    T x, y;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
