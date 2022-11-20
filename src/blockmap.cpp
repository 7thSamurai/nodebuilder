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

#include "blockmap.hpp"
#include "box.hpp"
#include "common.hpp"
#include "renderer.hpp"

BlockMap::BlockMap(Map &map) : map_(map) {
    width  = (map_.size().x + block_size - 1) / block_size;
    height = (map_.size().y + block_size - 1) / block_size;
}

void BlockMap::build(Renderer &renderer) {
    // Draw a grid representing the block map
    auto draw_grid = [&]() {
        renderer.clear();
        const Color color(0x20, 0x20, 0x20);

        // Draw the horizontal lines
        Vec2f pos(map_.offset().x, map_.offset().y);
        for (auto y = 0; y <= height; y++) {
            renderer.draw_line(Linef(pos, pos + Vec2f(map_.size().x, 0.0f)), color);
            pos.y += block_size;
        }

        // Draw the vertical lines
        pos = Vec2f(map_.offset().x, map_.offset().y);
        for (auto x = 0; x <= width; x++) {
            renderer.draw_line(Linef(pos, pos + Vec2f(0.0f, map_.size().y)), color);
            pos.x += block_size;
        }

        renderer.draw_map_outline();
    };

    // Generate the blocks
    for (int y = height-1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            draw_grid();
            gen(x, y, renderer);
            renderer.show();
        }
    }
}

void BlockMap::save() {
    std::vector<std::uint16_t> data;

    // Fill in the header
    data.push_back(Common::little16(map_.offset().x));
    data.push_back(Common::little16(map_.offset().y));
    data.push_back(Common::little16(width));
    data.push_back(Common::little16(height));

    // Make space for the offsets
    data.insert(data.end(), width*height, 0x0000);

    for (auto [list, blocks] : lists) {
        // Update the offsets
        for (auto block : blocks)
            data[header_size + block] = Common::little16(data.size());

        data.push_back(0x0000); // Start of list marker

        // Save the list
        for (auto line : list)
            data.push_back(Common::little16(line));

        data.push_back(0xffff); // End of list marker
    }

    map_.replace_blockmap(&data[0], data.size());
}

void BlockMap::gen(unsigned int x, unsigned int y, Renderer &renderer) {
    // Create a bounding box for this block
    auto box = Boxf(
        Vec2f(map_.offset().x + x*block_size, map_.offset().y + y*block_size),
        Vec2f(map_.offset().x + x*block_size+block_size, map_.offset().y + y*block_size+block_size)
    );

    auto vertices = map_.get_vertices();
    auto linedefs = map_.get_linedefs();

    List list;

    // Look at each linedef
    for (auto i = 0; i < map_.num_linedefs(); i++) {
        auto p1 = Vec2f(vertices[linedefs[i].start].x, vertices[linedefs[i].start].y);
        auto p2 = Vec2f(vertices[linedefs[i].end].x, vertices[linedefs[i].end].y);

        // Check if the block contains the linedef
        if (box.contains(Linef(p1, p2)))
            list.push_back(i);
    }

    // Add the list
    lists[list].push_back(y*width + x);

    if (renderer.drawing()) {
        const auto color = Color::random();

        // Draw the lines that are inside the box
        for (const auto &i : list) {
            auto p1 = Vec2f(vertices[linedefs[i].start].x, vertices[linedefs[i].start].y);
            auto p2 = Vec2f(vertices[linedefs[i].end].x, vertices[linedefs[i].end].y);

            auto line = box.clip(Linef(p1, p2));
            renderer.add_line(line, color);
        }

        renderer.draw_box(box);
    }
}
