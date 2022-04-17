#pragma once

#include "seg.hpp"
#include <vector>

class Node;
class Map;
class Renderer;

class Bsp
{
public:
    Bsp(Map &map);
    ~Bsp();

    void build(Renderer &renderer);
    void save();

private:
    std::vector<Seg> create_segs();

    Map &map_;
    Node *root;
};
