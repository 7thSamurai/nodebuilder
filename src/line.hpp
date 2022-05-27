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

#pragma once

#include "vec.hpp"
#include "common.hpp"
#include <algorithm>
#include <iostream>

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

    Line operator + (const Vec2<T> &v) const {
        return Line(a + v, b + v);
    }

    Line operator - (const Vec2<T> &v) const {
        return Line(a - v, b - v);
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

    /**
     * Finds the exact point of intersection of this line segment and another line segment (This assumes that it does intersect)
     * @param l The line to check against
     * @return The point of intersection
     */
    Vec2<T> intersect_at(const Line<T> &l) const {
        // Derived from "y - y1 = m(x - x1)"
        T a1 = b.y - a.y;
        T b1 = a.x - b.x;
        T c1 = a1 * a.x + b1 * a.y;

        T a2 = l.b.y - l.a.y;
        T b2 = l.a.x - l.b.x;
        T c2 = a2 * l.a.x + b2 * l.a.y;

        float det = a1 * b2 - a2 * b1;

        if (det == 0) {
            std::cerr << "Error lines are parallel" << std::endl;
            return Vec2<T>(0, 0);
        }

        T x = (b2 * c1 - b1 * c2) / det;
        T y = (a1 * c2 - a2 * c1) / det;

        return Vec2<T>(x, y);
    }

    Vec2<T> a, b;
};

using Linei = Line<int>;
using Linef = Line<float>;
