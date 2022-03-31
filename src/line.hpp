#pragma once

#include "line.hpp"

class Line
{
public:
    Line() {
    }

    Line(const Point &a, const Point &b) {
        this->a = a;
        this->b = b;
    }
    
    bool operator == (const Line &l) const {
        return a == l.a && b == l.b;
    }
    
    bool operator != (const Line &l) const {
        return a != l.a || b != l.b;
    }
    
    int rise() const {
        return b.y - a.y;
    }
    
    int run() const {
        return b.x - a.x;
    }
    
    int slope() const {
        return static_cast<float>(rise()) / static_cast<float>(run());
    }
    
    Point a, b;
}; 
