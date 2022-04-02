#pragma once

#include "line.hpp"

class Splitter
{
public:
    Splitter();
    Splitter(const Linef &line);

    /**
     * Finds the exact point of intersection of this splitter and a line (This assumes that it does intersect)
     * @param l The line to check against
     * @return The point of intersection
     */
    Vec2f intersect_at(const Linef &l) const;
    
    /**
     * Determines what side of this splitter a point is on
     * @param pt The point to check
     * @return -1 if on left, 0 if collinear, 1 if on right
     */
    int side_of(const Vec2f &pt) const;
    
    /**
     * Determines what side of this splitter a line is on
     * @param l The line to check
     * @return -1 if on left, 0 if intersects, 1 if on right
     */
    int side_of(const Linef &l) const;

    Vec2f p;  // Start Point
    float dx; // Delta X
    float dy; // Delta Y
};
