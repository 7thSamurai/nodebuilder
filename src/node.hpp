#pragma once

#include "splitter.hpp"
#include "box.hpp"
#include "seg.hpp"
#include "polygon.hpp"
#include <vector>

class Renderer;

class Node
{
public:
    Node();
    Node(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer);
    ~Node();

    void create(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer);

    const Node *left () const { return left_; }
    const Node *right() const { return right_; }
    const std::vector<Seg> segs() const { return segs_; }

    Splitter splitter() const { return splitter_; }
    Boxf bounds() const { return bounds_; }
    bool leaf() const { return !segs_.empty(); }

private:
    int splitter_score(const std::vector<Seg> &segs, unsigned int splitter_index) const;
    void split(const std::vector<Seg> &segs, unsigned int splitter_index, std::vector<Seg> &front_segs, std::vector<Seg> &back_segs);
    Polyf carve(const std::vector<Seg> &segs, const Polyf &poly);

    Node *left_, *right_;

    Splitter splitter_;
    Boxf bounds_;
    std::vector<Seg> segs_; // If this is a leaf node
};
