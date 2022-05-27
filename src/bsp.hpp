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
