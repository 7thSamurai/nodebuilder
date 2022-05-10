#pragma once

#include "map.hpp"
#include "seg.hpp"
#include <vector>

class Node;
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
    std::size_t unique_vertex(int x, int y);

    void process_linedefs();
    int process_ssector(const Node *node);
    int process_node(const Node *node);

    Map &map_;
    Node *root;

    std::vector<Map::Vertex> vertices;
    std::vector<Map::LineDef> linedefs;
    std::vector<Map::Seg> segs;
    std::vector<Map::SSector> ssectors;
    std::vector<Map::Node> nodes;
};
