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

    std::size_t size() const {
        return points_.size();
    }

    std::vector<Vec2<T>> &points() {
        return points_;
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
