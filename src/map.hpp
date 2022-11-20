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

#include "box.hpp"
#include "vec.hpp"
#include <string>
#include <memory>
#include <algorithm>

class Wad;

class Map
{
public:
    struct Thing {
        std::int16_t x;
        std::int16_t y;
        std::uint16_t angle;
        std::uint16_t type;
        std::uint16_t flags;
    };

    struct LineDef {
        std::uint16_t start;
        std::uint16_t end;
        std::uint16_t flags;
        std::uint16_t type;
        std::uint16_t tag;
        std::uint16_t sidedef[2];
    };

    struct SideDef {
        std::int16_t x;
        std::int16_t y;
        char upper[8];
        char lower[8];
        char middle[8];
        std::uint16_t sector;
    };

    struct Vertex {
        std::int16_t x;
        std::int16_t y;
    };

    struct Seg {
        std::uint16_t start;
        std::uint16_t end;
        std::uint16_t angle;
        std::uint16_t linedef;
        std::uint16_t dir;
        std::uint16_t offset;
    };

    struct SSector {
        std::uint16_t count;
        std::uint16_t first;
    };

    struct Node {
        std::int16_t x;
        std::int16_t y;
        std::int16_t dx;
        std::int16_t dy;
        std::int16_t lbounds[4];
        std::int16_t rbounds[4];
        std::uint16_t child[2];
    };

    struct Sector {
        std::int16_t floorh;
        std::int16_t ceilh;
        char floor_tex[8];
        char ceil_tex[8];
        std::uint16_t light;
        std::uint16_t special;
        std::uint16_t tag;
    };

    struct Reject {
    };

    using BlockMap = std::uint16_t;

    Map(const std::string &map, Wad &wad);

    bool load();
    void save();

    bool valid() const;

    Boxi bounds () const;
    Vec2i offset() const;
    Vec2i size  () const;
    std::string map() const;

    std::size_t num_things() const   { return things_.num(); }
    std::size_t num_linedefs() const { return linedefs_.num();  }
    std::size_t num_sidedefs() const { return sidedefs_.num(); }
    std::size_t num_vertices() const { return vertices_.num(); }
    std::size_t num_segs() const     { return segs_.num(); }
    std::size_t num_ssectors() const { return ssectors_.num(); }
    std::size_t num_nodes() const    { return nodes_.num(); }
    std::size_t num_sectors() const  { return sectors_.num(); }
    std::size_t num_reject()  const  { return reject_.num(); }
    std::size_t num_blockmap() const { return blockmap_.num(); }

    auto get_things() const   { return things_.get(); }
    auto get_linedefs() const { return linedefs_.get(); }
    auto get_sidedefs() const { return sidedefs_.get(); }
    auto get_vertices() const { return vertices_.get(); }
    auto get_segs() const     { return segs_.get(); }
    auto get_ssectors() const { return ssectors_.get(); }
    auto get_nodes() const    { return nodes_.get(); }
    auto get_sectors() const  { return sectors_.get(); }
    auto get_reject()  const  { return reject_.get(); }
    auto get_blockmap() const { return blockmap_.get(); }

    void replace_things(const Thing *things, std::size_t num)        { things_  .replace(things, num); }
    void replace_linedefs(const LineDef *linedefs, std::size_t num)  { linedefs_.replace(linedefs, num); }
    void replace_sidedefs(const SideDef *sidedefs, std::size_t num)  { sidedefs_.replace(sidedefs, num); }
    void replace_vertices(const Vertex *vertices, std::size_t num)   { vertices_.replace(vertices, num); }
    void replace_segs(const Seg *segs, std::size_t num)              { segs_    .replace(segs, num); }
    void replace_ssectors(const SSector *ssectors, std::size_t num)  { ssectors_.replace(ssectors, num); }
    void replace_nodes(const Node *nodes, std::size_t num)           { nodes_   .replace(nodes, num); }
    void replace_sectors(const Sector *sectors, std::size_t num)     { sectors_ .replace(sectors, num); }
    void replace_reject(const Reject *reject, std::size_t num)       { reject_  .replace(reject, num); }
    void replace_blockmap(const BlockMap *blockmap, std::size_t num) { blockmap_.replace(blockmap, num); }

private:
	template <typename T>
    struct MapLump {
    	std::size_t num() const {
    		return size / sizeof(T);
    	}

    	const T *get() const {
    		return reinterpret_cast<const T*>(data.get());
    	}

        void replace(const T *data, std::size_t num) {
            this->changed = true;
            this->size    = num * sizeof(T);
            this->data    = std::make_unique<std::uint8_t[]>(size);

            std::copy_n(reinterpret_cast<const std::uint8_t*>(data), this->size, this->data.get());
        }

        bool changed;
        std::size_t size;
        std::unique_ptr<std::uint8_t[]> data;
    };

    void find_bounds();
    void swap_byte_order();

    Wad &wad_;
    std::string map_;
    Boxi bounds_;

    MapLump<Thing> things_;
    MapLump<LineDef> linedefs_;
    MapLump<SideDef> sidedefs_;
    MapLump<Vertex> vertices_;
    MapLump<Seg> segs_;
    MapLump<SSector> ssectors_;
    MapLump<Node> nodes_;
    MapLump<Sector> sectors_;
    MapLump<Reject> reject_;
    MapLump<BlockMap> blockmap_;
};
