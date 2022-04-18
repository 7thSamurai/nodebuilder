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

    /*std::vector<Seg> segs;
    segs.push_back(Seg(Vec2f(1376,-3360), Vec2f(1376,-3339), false, 0, 0));
    segs.push_back(Seg(Vec2f(1544,-3384), Vec2f(1784,-3448), false, 0, 0));
    segs.push_back(Seg(Vec2f(2026,-3648), Vec2f(1984,-3648), false, 0, 0));
    segs.push_back(Seg(Vec2f(1984,-3648), Vec2f(1376,-3648), false, 0, 0));
    segs.push_back(Seg(Vec2f(1376,-3648), Vec2f(1376,-3520), false, 0, 0));
    segs.push_back(Seg(Vec2f(1376,-3392), Vec2f(1376,-3360), false, 0, 0));
    segs.push_back(Seg(Vec2f(1376,-3520), Vec2f(1376,-3392), false, 0, 0));*/


    Polyf poly;
    poly.add(Vec2f(map_.bounds().min().x, map_.bounds().min().y));
    poly.add(Vec2f(map_.bounds().min().x, map_.bounds().max().y));
    poly.add(Vec2f(map_.bounds().max().x, map_.bounds().max().y));
    poly.add(Vec2f(map_.bounds().max().x, map_.bounds().min().y));

    root = new Node(segs, poly, renderer);
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

        segs.push_back(Seg(p1, p2, false, 0, i));

        // Two sided
        if (linedefs->flags & 0b100)
            segs.push_back(Seg(p2, p1, true, 0, i));
    }

    return segs;
}
