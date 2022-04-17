#pragma once

#include "splitter.hpp"
#include "box.hpp"
#include "seg.hpp"
#include <vector>

class Renderer;

class Node
{
public:
    Node();
    Node(const std::vector<Seg> &segs, Renderer &renderer);
    ~Node();

    void create(const std::vector<Seg> &segs, Renderer &renderer);

    Boxf bounds() const { return bounds_; }

private:
    int splitter_score(const std::vector<Seg> &segs, unsigned int splitter_index) const;
    void split(const std::vector<Seg> &segs, unsigned int splitter_index, std::vector<Seg> &front_segs, std::vector<Seg> &back_segs);

    Node *left, *right;

    Splitter splitter_;
    Boxf bounds_;
    std::vector <Seg> segs_; // If this is a leaf node
};
