#include "blockmap.hpp"
#include "box.hpp"
#include "common.hpp"

BlockMap::BlockMap(Map &map) : map_(map) {
    width  = (map_.size().x + block_size - 1) / block_size;
    height = (map_.size().y + block_size - 1) / block_size;

    blocks = std::make_unique<BlockList[]>(width * height);
}

void BlockMap::build() {
    // Generate the blocks
    for (auto y = 0; y < map_.size().y; y++) {
        for (auto x = 0; x < map_.size().x; x++)
            gen(x, y);
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
    data.insert(data.end(), width*height, 0);

    unsigned int offset = header_size + width*height;
    unsigned int index = 0;

    // Save the blocks
    for (auto y = 0; y < height; y++) {
        for (auto x = 0; x < width; x++) {
            // Update the offset
            const auto &block = blocks[y*width + x];
            data[header_size + index++] = Common::little16(offset);
            offset += block.size() + 2;

            data.push_back(0); // Start of list marker

            for (auto i : block)
                data.push_back(Common::little16(i));

            data.push_back(-1); // End of list marker
        }
    }
}

void BlockMap::gen(unsigned int x, unsigned int y) {
    // Create a bounding box for this block
    auto box = Boxf(
        Vec2f(map_.offset().x + x*block_size, map_.offset().x + y*block_size),
        Vec2f(map_.offset().y + x*block_size+block_size, map_.offset().y + y*block_size+block_size)
    );

    auto &block = blocks[y*width + x];

    auto vertices = map_.get_vertices();
    auto linedefs = map_.get_linedefs();

    // Look at each linedef
    for (auto i = 0; i < map_.num_linedefs(); i++, linedefs++) {
        auto p1 = Vec2f(vertices[linedefs->start].x, vertices[linedefs->start].y);
        auto p2 = Vec2f(vertices[linedefs->end].x, vertices[linedefs->end].y);

        // Check if the block contains the linedef
        if (box.contains(Linef(p1, p2)))
            block.push_back(i);
    }
}
