#include "splitter.hpp"
#include "common.hpp"
#include <iostream>

Splitter::Splitter() : p(), dx(0.0f), dy(0.0f) {
}

Splitter::Splitter(const Seg &seg) {
    p  = seg.p1();
    dx = seg.p2().x - seg.p1().x;
    dy = seg.p2().y - seg.p1().y;
}

Vec2f Splitter::intersect_at(const Seg &seg) const {
    // Derived from "y - y1 = m(x - x1)"
    float a1 = dy;
    float b1 = -dx;
    float c1 = dy * p.x - dx * p.y;

    float a2 = seg.p2().y - seg.p1().y;
    float b2 = seg.p1().x - seg.p2().x;
    float c2 = a2 * seg.p1().x + b2 * seg.p1().y;

    float det = a1 * b2 - a2 * b1;

    if (det == 0.0f) {
        std::cerr << "Error lines are parallel" << std::endl;
        return Vec2f(0, 0);
    }

    float x = (b2 * c1 - b1 * c2) / det;
    float y = (a1 * c2 - a2 * c1) / det;

    return Vec2f(static_cast<int>(x), static_cast<int>(y));
}

std::pair<Seg, Seg> Splitter::cut(const Seg &seg) const {
    Seg l1, l2;

    Vec2f p  = intersect_at(seg);
    int side = side_of(seg.p1());

    if (side == -1) {
        float offset = std::sqrt(std::pow(p.x - seg.p1().x, 2) + std::pow(p.y - seg.p1().y, 2));
        l1 = Seg(seg.p1(), p, seg.side(), seg.offset(), seg.linedef());
        l2 = Seg(p, seg.p2(), seg.side(), seg.offset()+offset, seg.linedef());
    }

    else {
        float offset = std::sqrt(std::pow(p.x - seg.p2().x, 2) + std::pow(p.y - seg.p2().y, 2));
        l1 = Seg(p, seg.p2(), seg.side(), seg.offset()+offset, seg.linedef());
        l2 = Seg(seg.p1(), p, seg.side(), seg.offset(), seg.linedef());
    }

    return std::make_pair(l1, l2);
}

int Splitter::side_of(const Vec2f &pt) const {
    // Cross-Product
    return  Common::sign(dx * (pt.y - p.y) - dy * (pt.x - p.x));
}

int Splitter::side_of(const Seg &seg) const {
    int s1 = side_of(seg.p1());
    int s2 = side_of(seg.p2());

    if (s1 == s2) {
        // Check if the line is collinear with the splitter
        if (!s1) {
            float dx2 = seg.p2().x - seg.p1().x;
            float dy2 = seg.p2().y - seg.p1().y;

            // If the line is going the same direction as the splitter, put it on the left
            if (Common::sign(dx) == Common::sign(dx2) && Common::sign(dy) == Common::sign(dy2))
                return -1; // Left
            else
                return 1; // Right
        }

        return s1;
    }

    // If one of the points is collinear with the splitter, use the other point
    if (!s1) return s2;
    if (!s2) return s1;

    return 0; // It intersects
}
