#pragma once

#include "map.hpp"
#include <vector>
#include <memory>

class BlockMap
{
public:
    BlockMap(Map &map);

    void build();
    void save();

private:
    using BlockList = std::vector<std::uint16_t>;

    const unsigned int header_size = 4;
    const unsigned int block_size  = 128;

    // Generate a block
    void gen(unsigned int x, unsigned int y);

    Map &map_;
    unsigned int width, height;

    std::unique_ptr<BlockList[]> blocks;
};
