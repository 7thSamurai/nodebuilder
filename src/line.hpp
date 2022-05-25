#pragma once

#include "vec.hpp"
#include "common.hpp"
#include <algorithm>

template <typename T>
class Line
{
public:
    Line() {
    }

    Line(const Vec2<T> &a, const Vec2<T> &b) {
        this->a = a;
        this->b = b;
    }

    bool operator == (const Line &l) const {
        return a == l.a && b == l.b;
    }

    bool operator != (const Line &l) const {
        return a != l.a || b != l.b;
    }

    T rise() const {
        return b.y - a.y;
    }

    T run() const {
        return b.x - a.x;
    }

    float slope() const {
        return static_cast<float>(rise()) / static_cast<float>(run());
    }

    int side_of(const Vec2<T> &p) const {
        return Common::sign((b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x));
    }

    /**
     * Checks if two line segments intersect
     * @param l The line to check against
     * @return true if they intersect
     */
    bool intersects(const Line<T> &l) const {
        // Find the bounding box of this line
        const Vec2<T> box1[] = {
            Vec2<T>(std::min(a.x, b.x), std::min(a.y, b.y)),
            Vec2<T>(std::max(a.x, b.x), std::max(a.y, b.y))
        };

        // Find the bounding box of the other line
        const Vec2<T> box2[] = {
            Vec2<T>(std::min(l.a.x, l.b.x), std::min(l.a.y, l.b.y)),
            Vec2<T>(std::max(l.a.x, l.b.x), std::max(l.a.y, l.b.y))
        };

        // Check if the two bounding boxes intersect
        if (box1[0].x <= box2[1].x && box1[1].x >= box2[0].x &&
            box1[0].y <= box2[1].y && box1[1].y >= box2[0].y)
            return side_of(l.a) != side_of(l.b) && l.side_of(a) != l.side_of(b);

        return false;
    }

    Vec2<T> a, b;
};

using Linei = Line<int>;
using Linef = Line<float>;
