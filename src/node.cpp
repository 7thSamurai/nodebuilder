#include "node.hpp"
#include "renderer.hpp"
#include <iostream>
#include <climits>
#include <SDL2/SDL.h>

Node::Node() : left_(nullptr), right_(nullptr) {
}

Node::Node(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer) : left_(nullptr), right_(nullptr) {
    create(segs, poly, renderer);
}

Node::~Node() {
    if (left_)  delete left_;
    if (right_) delete right_;
}

void Node::create(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer) {
    if (!renderer.running())
        return;

    // Find the bounding box of this node
    bounds_ = Boxf(segs[0].p1(), segs[0].p1());
    for (const auto &seg :segs) {
        bounds_.extend(seg.p1());
        bounds_.extend(seg.p2());
    }

    renderer.draw_map();

    for (const auto &seg : segs)
        renderer.draw_line(seg.line());

    renderer.show();
    renderer.draw_poly(poly);
    renderer.show();

    int best_score = INT_MAX;
    int splitter   = 0;

    // Find the best splitter
    for (auto i = 0; i < segs.size(); i++) {
        int score = splitter_score(segs, i);

        if (score < best_score) {
            best_score = score;
            splitter   = i;
        }
    }

    // If no lines where split, then this is a leaf node
    if (best_score == INT_MAX) {
        segs_  = segs;
        renderer.add_poly(carve(segs, poly), Color::random());

        return;
    }

    renderer.draw_splitter(Splitter(segs[splitter]));
    renderer.show();

    // Now actually split the node
    std::vector<Seg> front_segs, back_segs;
    split(segs, splitter, front_segs, back_segs);
    auto polys = Splitter(segs[splitter]).cut(poly);

    left_  = new Node(front_segs, polys.second, renderer);
    right_ = new Node(back_segs, polys.first, renderer);
}

int Node::splitter_score(const std::vector<Seg> &segs, unsigned int splitter_index) const {
    Splitter splitter(segs[splitter_index]);

    int front_count = 0;
    int back_count  = 0;

    for (auto i = 0; i < segs.size(); i++) {
        if (i == splitter_index) {
            front_count++;
            continue;
        }

        int side = splitter.side_of(segs[i]);

        if (side == -1)
            front_count++;
        else if (side == 1)
            back_count++;
        else {
            front_count++;
            back_count++;
        }
    }

    // No lines intersect
    if (!front_count || !back_count)
        return INT_MAX;

    int new_lines = front_count + back_count - segs.size();
    int diff = std::abs(front_count - back_count);

    return diff + new_lines*8;
}

void Node::split(const std::vector<Seg> &segs, unsigned int splitter_index, std::vector<Seg> &front_segs, std::vector<Seg> &back_segs) {
    Splitter splitter(segs[splitter_index]);

    for (auto i = 0; i < segs.size(); i++) {
        if (i == splitter_index) {
            front_segs.push_back(segs[i]);
            continue;
        }

        int side = splitter.side_of(segs[i]);

        if (side == -1)
            front_segs.push_back(segs[i]);
        else if (side == 1)
            back_segs.push_back(segs[i]);
        else {
            auto new_lines = splitter.cut(segs[i]);
            front_segs.push_back(Seg(new_lines.first));
            back_segs .push_back(Seg(new_lines.second));
        }
    }
}

Polyf Node::carve(const std::vector<Seg> &segs, const Polyf &poly) {
    Polyf carved = poly;

    for (const auto &seg : segs) {
        auto polys = Splitter(seg).cut(carved);
        carved = polys.second;
    }

    return carved;
}
