#pragma once

#include "map.hpp"
#include <vector>
#include <map>

class BlockMap
{
public:
    BlockMap(Map &map);

    void build();
    void save();

private:
    using List   = std::vector<std::uint16_t>;
    using Blocks = std::vector<unsigned int>;

    const unsigned int header_size = 4;
    const float block_size = 128;

    // Generate a block
    void gen(unsigned int x, unsigned int y);

    Map &map_;
    unsigned int width, height;

    std::map<List, Blocks> lists;
};
