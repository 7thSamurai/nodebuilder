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

Vec2f Splitter::intersect_at(const Linef &l) const {
    // Derived from "y - y1 = m(x - x1)"
    float a1 = dy;
    float b1 = -dx;
    float c1 = dy * p.x - dx * p.y;

    float a2 = l.b.y - l.a.y;
    float b2 = l.a.x - l.b.x;
    float c2 = a2 * l.a.x + b2 * l.a.y;

    float det = a1 * b2 - a2 * b1;

    if (det == 0.0f) {
        std::cerr << "Error lines are parallel" << std::endl;
        return Vec2f(0, 0);
    }

    float x = (b2 * c1 - b1 * c2) / det;
    float y = (a1 * c2 - a2 * c1) / det;

    return Vec2f(static_cast<int>(x), static_cast<int>(y));
}

Vec2f Splitter::intersect_at(const Seg &seg) const {
    return intersect_at(seg.line());
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

std::pair<Polyf, Polyf> Splitter::cut(const Polyf &poly) const {
    Polyf left, right;

    for (auto i = 0; i < poly.size(); i++) {
        auto start = poly.at(i + 0);
        auto end   = poly.at(i + 1);

        int start_side = side_of(start);
        int end_side   = side_of(end);

        // If the line ends on the splitter
        if (end_side == 0) {
            left.add(end);
            right.add(end);
            continue;
        }

        // If the line is completely to one side of the splitter
        if (start_side == end_side) {
            if (start_side == -1)
                right.add(end);
            else
                left.add(end);
            continue;
        }

        // Else it intersects
        auto p = intersect_at(Linef(start, end));

        // If the line ends on the left of the splitter
        if (end_side == -1) {
            left.add(p);
            right.add(p);
            right.add(end);
        }

        // Or if the line ends on the right of the splitter
        else {
            right.add(p);
            left.add(p);
            left.add(end);
        }
    }

    return std::make_pair(left, right);
}

int Splitter::side_of(const Vec2f &pt) const {
    if (!this->dx) {
        if (pt.x > p.x-2 && pt.x < p.x+2)
            return 0;
        if (pt.x < p.x)
            return this->dy > 0 ? 1 : -1;

        return this->dy < 0 ? 1 : -1;
    }
    if (!this->dy) {
        if (pt.y > p.y-2 && pt.y < p.y+2)
            return 0;
        if (pt.y < p.y)
            return this->dx < 0 ? 1 : -1;

        return this->dx > 0 ? 1 : -1;
    }

    float dx = p.x - pt.x;
    float dy = p.y - pt.y;
    float a  = this->dx*this->dx + this->dy*this->dy;
    float b  = 2 * (this->dx*dx + this->dy*dy);
    float c  = dx*dx+dy*dy - 2*2;
    float d  = b*b - 4*a*c;

    if (d > 0)
        return 0;

    dx = pt.x - p.x;
    dy = pt.y - p.y;

    float left  = dx * this->dy;
    float right = dy * this->dx;

    if (std::abs(left - right) < 0.5f)
        return 0;
    if (right < left)
        return -1;

    return 1;
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
