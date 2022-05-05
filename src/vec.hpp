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

    Vec2 operator + (const Vec2 &v) const { return Vec2(x + v.x, y + v.y); }
    Vec2 operator - (const Vec2 &v) const { return Vec2(x - v.x, y - v.y); }
    Vec2 operator * (const Vec2 &v) const { return Vec2(x * v.x, y * v.y); }
    Vec2 operator / (const Vec2 &v) const { return Vec2(x / v.x, y / v.y); }

    Vec2 operator + (const T &n) const { return Vec2(x + n, y + n); }
    Vec2 operator - (const T &n) const { return Vec2(x - n, y - n); }
    Vec2 operator * (const T &n) const { return Vec2(x * n, y * n); }
    Vec2 operator / (const T &n) const { return Vec2(x / n, y / n); }

    bool operator == (const Vec2 &v) const {
        return x == v.x && y == v.y;
    }

    bool operator != (const Vec2 &v) const {
        return x != v.x || y != v.y;
    }

    T x, y;
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;
