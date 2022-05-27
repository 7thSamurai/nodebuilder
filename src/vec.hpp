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
