#pragma once

#include "box.hpp"
#include "vec.hpp"
#include <string>
#include <memory>

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

    struct BlockMap {
        std::int16_t x;
        std::int16_t y;
        std::uint16_t cols;
        std::uint16_t rows;
    };

    Map(const std::string &map, Wad &wad);

    bool load();
    void save();

    bool valid() const;

    Vec2i offset() const;
    Vec2i size  () const;

    std::size_t num_things() const      { return things_.size   / sizeof(Thing); }
    std::size_t num_linedefs() const    { return linedefs_.size / sizeof(LineDef); }
    std::size_t num_sidedefs() const    { return sidedefs_.size / sizeof(SideDef); }
    std::size_t num_vertices() const    { return vertices_.size / sizeof(Vertex); }
    std::size_t num_segs() const        { return segs_.size     / sizeof(Seg); }
    std::size_t num_ssectors() const    { return ssectors_.size / sizeof(SSector); }
    std::size_t num_nodes() const       { return nodes_.size    / sizeof(Node); }
    std::size_t num_sectors() const     { return sectors_.size  / sizeof(Sector); }
    std::size_t num_reject()  const     { return reject_.size   / sizeof(Reject); }
    std::size_t num_blockmap() const    { return blockmap_.size / sizeof(BlockMap); }

    const Thing *get_things() const         { return reinterpret_cast<const Thing*>(things_.data.get()); }
    const LineDef *get_linedefs() const     { return reinterpret_cast<const LineDef*>(linedefs_.data.get()); }
    const SideDef *get_sidedefs() const     { return reinterpret_cast<const SideDef*>(sidedefs_.data.get()); }
    const Vertex *get_vertices() const      { return reinterpret_cast<const Vertex*>(vertices_.data.get()); }
    const Seg *get_segs() const             { return reinterpret_cast<const Seg*>(segs_.data.get()); }
    const SSector *get_ssectors() const     { return reinterpret_cast<const SSector*>(ssectors_.data.get()); }
    const Node *get_nodes() const           { return reinterpret_cast<const Node*>(nodes_.data.get()); }
    const Sector *get_sectors() const       { return reinterpret_cast<const Sector*>(sectors_.data.get()); }
    const Reject *get_reject()  const       { return reinterpret_cast<const Reject*>(reject_.data.get()); }
    const BlockMap *get_blockmap() const    { return reinterpret_cast<const BlockMap*>(blockmap_.data.get()); }

private:
    struct MapLump {
        bool changed;
        std::size_t size;
        std::unique_ptr<std::uint8_t[]> data;
    };

    void find_bounds();
    void swap_byte_order();

    Wad &wad_;
    std::string map_;
    Boxi bounds;

    MapLump things_;
    MapLump linedefs_;
    MapLump sidedefs_;
    MapLump vertices_;
    MapLump segs_;
    MapLump ssectors_;
    MapLump nodes_;
    MapLump sectors_;
    MapLump reject_;
    MapLump blockmap_;
};
