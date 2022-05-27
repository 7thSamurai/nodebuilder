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

#include "seg.hpp"
#include "polygon.hpp"
#include <utility>

class Splitter
{
public:
    Splitter();
    Splitter(const Seg &seg);

    /**
     * Finds the exact point of intersection of this splitter and a line (This assumes that it does intersect)
     * @param l The line to check against
     * @return The point of intersection
     */
    Vec2f intersect_at(const Linef &l) const;

    /**
     * Finds the exact point of intersection of this splitter and a line (This assumes that it does intersect)
     * @param seg The line to check against
     * @return The point of intersection
     */
    Vec2f intersect_at(const Seg &seg) const;

    /**
     * Cuts a line that intersects with this splitter in two
     * @param seg The line to cut
     * @return The two new lines
     */
    std::pair<Seg, Seg> cut(const Seg &seg) const;

    /**
     * Cuts a polygon that intersects with this splitter in two
     * @param poly The polygon to cut
     * @return The two new polygons
     */
    std::pair<Polyf, Polyf> cut(const Polyf &poly) const;

    /**
     * Determines what side of this splitter a point is on
     * @param pt The point to check
     * @return -1 if on left, 0 if collinear, 1 if on right
     */
    int side_of(const Vec2f &pt) const;

    /**
     * Determines what side of this splitter a line is on
     * @param seg The line to check
     * @return -1 if on left, 0 if intersects, 1 if on right
     */
    int side_of(const Seg &seg) const;

    Vec2f p;  // Start Point
    float dx; // Delta X
    float dy; // Delta Y
};
