#pragma once

#include "vec.hpp"
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

private:
    std::vector<Vec2<T>> points_;
};

using Polyi = Poly<int>;
using Polyf = Poly<float>;
