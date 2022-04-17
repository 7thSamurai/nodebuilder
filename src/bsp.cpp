#include "bsp.hpp"
#include "map.hpp"
#include "node.hpp"

Bsp::Bsp(Map &map) : map_(map), root(nullptr) {
}

Bsp::~Bsp() {
    if (root)
        delete root;
}

void Bsp::build(Renderer &renderer) {
    auto segs = create_segs();
    root = new Node(segs, renderer);
}

void Bsp::save() {
    // TODO
}

std::vector<Seg> Bsp::create_segs() {
    std::vector<Seg> segs;

    auto vertices = map_.get_vertices();
    auto linedefs = map_.get_linedefs();

    for (auto i = 0; i < map_.num_linedefs(); i++, linedefs++) {
        auto p1 = Vec2f(vertices[linedefs->start].x, vertices[linedefs->start].y);
        auto p2 = Vec2f(vertices[linedefs->end].x, vertices[linedefs->end].y);

        segs.push_back(Seg(p1, p2, false, 0, 1));

        // Two sided
        if (linedefs->flags & 0b100)
            segs.push_back(Seg(p1, p2, true, 0, i));
    }

    return segs;
}
