#pragma once

#include "vec.hpp"
#include "common.hpp"

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

    Vec2<T> a, b;
};

using Linei = Line<int>;
using Linef = Line<float>;
