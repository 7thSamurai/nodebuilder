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
