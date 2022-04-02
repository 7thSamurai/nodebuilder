#include "splitter.hpp"
#include "common.hpp"
#include <iostream>

Splitter::Splitter() : p(), dx(0.0f), dy(0.0f) {
}

Splitter::Splitter(const Linef &line) {
    p  = line.a;
    dx = line.b.x - line.a.x;
    dy = line.b.y - line.a.y; 
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

int Splitter::side_of(const Vec2f &pt) const {
    // Cross-Product
    return  Common::sign(dx * (pt.y - p.y) - dy * (pt.x - p.x));
}

int Splitter::side_of(const Linef &l) const {
    int s1 = side_of(l.a);
    int s2 = side_of(l.b);

    if (s1 == s2) {
        // Check if the line is collinear with the splitter
        if (!s1) {
            float dx2 = l.b.x - l.a.x;
            float dy2 = l.b.y - l.a.y;
            
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
