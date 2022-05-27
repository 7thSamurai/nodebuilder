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
#include "line.hpp"

template <typename T>
class Box
{
public:
    Box() = default;
    Box(const Vec2<T> &min, const Vec2<T> &max) : min_(min), max_(max) {
    }

    bool operator == (const Box &b) const {
        return min_ == b.min_ && max_ == b.max_;
    }

    bool operator != (const Box &b) const {
        return min_ != b.min_ || max_ != b.max_;
    }

    /**
     * Extends the bounding-box so that a point fits inside
     * @param p The point to fit inside
     */
    void extend(const Vec2<T> &p) {
        if (p.x < min_.x)
            min_.x = p.x;
        else if (p.x > max_.x)
            max_.x = p.x;

        if (p.y < min_.y)
            min_.y = p.y;
        else if (p.y > max_.y)
            max_.y = p.y;
    }

    /**
     * Checks if a point is contained inside
     * @param p The point to check
     * @return true if contained
     */
    bool contains(const Vec2<T> &p) const {
        if (p.x < min_.x || p.x > max_.x) return false;
        if (p.y < min_.y || p.y > max_.y) return false;

        return true;
    }

    /**
     * Checks if a line is contained inside (At least partially)
     * @param l The line to check
     * @return true if contained
     */
    bool contains(const Line<T> &l) const {
        // First check if one of the points is contained
        if (contains(l.a) || contains(l.b))
            return true;

        // Then check if any of the edges intersect
        if (l.intersects(Line<T>(top_left (), top_right()))) return true;
        if (l.intersects(Line<T>(bot_left (), bot_right()))) return true;
        if (l.intersects(Line<T>(top_left (), bot_left ()))) return true;
        if (l.intersects(Line<T>(top_right(), bot_right()))) return true;

        return false;
    }

    /**
     * Clips a line so it fits inside
     * @param l The line to clip
     */
    Line<T> clip(Line<T> l) const {
        auto cut = [](const Line<T> &l1, const Line<T> &l2) {
            auto p   = l1.intersect_at(l2);
            int side = l1.side_of(l2.a);

            if (side == 1)
                return Line(l2.a, p);

            return Line(l2.b, p);
        };

        auto top   = Line<T>(top_left (), top_right());
        auto bot   = Line<T>(bot_right(), bot_left ());
        auto left  = Line<T>(bot_left (), top_left());
        auto right = Line<T>(top_right(), bot_right());

        if (l.intersects(top))   l = cut(top, l);
        if (l.intersects(bot))   l = cut(bot, l);
        if (l.intersects(left))  l = cut(left, l);
        if (l.intersects(right)) l = cut(right, l);

        return l;
    }

    Vec2<T> min() const { return min_; }
    Vec2<T> max() const { return max_; }

    // Gets the various corners
    Vec2<T> top_left () const { return Vec2<T>(min_.x, min_.y); }
    Vec2<T> top_right() const { return Vec2<T>(max_.x, min_.y); }
    Vec2<T> bot_left () const { return Vec2<T>(min_.x, max_.y); }
    Vec2<T> bot_right() const { return Vec2<T>(max_.x, max_.y); }

    Vec2<T> size() const { return max_ - min_; }
    T width () const { return max_.x - min_.x; }
    T height() const { return max_.y - min_.y; }

private:
    Vec2<T> min_, max_;
};

using Boxi = Box<int>;
using Boxf = Box<float>;
