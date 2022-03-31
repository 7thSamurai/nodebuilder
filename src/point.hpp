#pragma once

class Point
{
public:
    Point() {
        x = y = 0;
    }
    
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
    
    bool operator == (const Point &p) const {
        return x == p.x && y == p.y;
    }
    
    bool operator != (const Point &p) const {
        return x != p.x || y != p.y;
    }

    int x, y;
}; 
