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
    Node(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer, int &num_nodes, int &num_segs, int &num_ssectors);
    ~Node();

    void create(const std::vector<Seg> &segs, const Polyf &poly, Renderer &renderer, int &num_nodes, int &num_segs, int &num_ssectors);

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
