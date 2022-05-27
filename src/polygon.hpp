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
#include "box.hpp"
#include <vector>

template <typename T>
class Poly
{
public:
    void add(const Vec2<T> &p) {
        points_.push_back(p);
    }

    Vec2<T> at(std::size_t i) const {
        return points_.at(i % size());
    }

    Vec2<T> &at(std::size_t i) {
        return points_.at(i % size());
    }

    std::size_t size() const {
        return points_.size();
    }

    std::vector<Vec2<T>> &points() {
        return points_;
    }

    Vec2<T> operator [] (std::size_t i) const {
        return at(i);
    }

    Vec2<T> &operator [] (std::size_t i) {
        return at(i);
    }

    bool point_inside(const Vec2<T> &p) const {
        int pos = 0, neg = 0;

        for (auto i = 0; i < size(); i++) {
            if (at(i) == p)
                return true;

            int side = Line<T>(at(i+0), at(i+1)).side_of(p);

            if (side > 0) pos++;
            if (side < 0) neg++;

            if (pos && neg)
                return false;
        }

        return true;
    }

    Box<T> bounds() const {
        Box<T> bounds(points_[0], points_[0]);

        for (const auto &point : points_)
            bounds.extend(point);

        return bounds;
    }

private:
    std::vector<Vec2<T>> points_;
};

using Polyi = Poly<int>;
using Polyf = Poly<float>;
