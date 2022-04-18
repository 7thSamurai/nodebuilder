#include "map.hpp"
#include "wad.hpp"
#include "common.hpp"
#include <iostream>

Map::Map(const std::string &map, Wad &wad) : map_(map), wad_(wad) {
}

bool Map::load() {
    auto read_entry = [&](const std::string &name, MapLump &lump, bool required) -> bool {
        lump.data    = wad_.read_map_lump(map_, name, lump.size);
        lump.changed = false;

        return !required || lump.data;
    };

    // Read all the map entries
    bool valid = true;
    valid &= read_entry("THINGS",   things_,    true);
    valid &= read_entry("LINEDEFS", linedefs_,  true);
    valid &= read_entry("SIDEDEFS", sidedefs_,  true);
    valid &= read_entry("VERTEXES", vertices_,  true);
    valid &= read_entry("SEGS",     segs_,      false);
    valid &= read_entry("SSECTORS", ssectors_,  false);
    valid &= read_entry("SECTORS",  sectors_,   true);
    valid &= read_entry("NODES",    nodes_,     false);
    valid &= read_entry("REJECT",   reject_,    false);
    valid &= read_entry("BLOCKMAP", blockmap_,  false);

    swap_byte_order();
    find_bounds();

    return valid;
}

void Map::save() {
    auto save_entry = [&](const std::string &name, const std::string &after, MapLump &lump) {
        if (!lump.data || !lump.changed)
            return;

        lump.changed = false;

        if (!wad_.write_map_lump(map_, name, lump.data.get(), lump.size)) {
            if (after.size()) // Not required
                wad_.insert_map_lump(map_, after, name, lump.data.get(), lump.size);
            else
                std::cerr << "Map " << map_ << " is missing required lump " << name << std::endl;
        }
    };

    swap_byte_order();

    // Save the entries
    save_entry("THINGS",   "",          things_);
    save_entry("LINEDEFS", "",          linedefs_);
    save_entry("SIDEDEFS", "",          sidedefs_);
    save_entry("VERTEXES", "",          vertices_);
    save_entry("SEGS",     "SECTORS",   segs_);
    save_entry("SSECTORS", "SEGS",      ssectors_);
    save_entry("NODES",    "SSECTORS",  nodes_);
    save_entry("SECTORS",  "VERTEXES",  sectors_);
    save_entry("REJECT",   "NODES",     reject_);
    save_entry("BLOCKMAP", "REJECT",    blockmap_);

    swap_byte_order();
}

bool Map::valid() const {
    bool valid = true;

    // Check the line-defs
    auto linedefs = get_linedefs();
    for (auto i = 0; i < num_linedefs(); i++, linedefs++) {
        if (linedefs->start >= num_vertices()) {
            valid = false;
        }
        if (linedefs->end >= num_vertices()) {
            valid = false;
        }

        if (linedefs->sidedef[0] != -1) {
            if (linedefs->sidedef[0] >= num_sidedefs()) {
                valid = false;
            }
        }
        if (linedefs->sidedef[1] != -1) {
            if (linedefs->sidedef[1] >= num_sidedefs()) {
                valid = false;
            }
        }
    }

    // Check the side-defs
    auto sidedefs = get_sidedefs();
    for (auto i = 0; i < num_sidedefs(); i++, sidedefs++) {
        if (sidedefs->sector >= num_sectors()) {
            valid = false;
        }
    }

    return valid;
}

Boxi Map::bounds() const {
    return bounds_;
}

Vec2i Map::offset() const {
    return bounds_.min();
}

Vec2i Map::size() const {
    return bounds_.max() - bounds_.min();
}

void Map::find_bounds() {
    if (!num_vertices()) {
        bounds_ = Box(Vec2i(0, 0), Vec2i(0, 0));
        return;
    }

    auto vertices = get_vertices();
    bounds_ = Boxi(Vec2i(vertices[0].x, vertices[0].y), Vec2i(vertices[0].x, vertices[0].y));

    for (auto i = 1; i < num_vertices(); i++) {
        bounds_.extend(Vec2i(vertices[i].x, vertices[i].y));
    }
}

void Map::swap_byte_order() {
#if BYTE_ORDER == BIG_ENDIAN
    auto things = reinterpret_cast<Thing*>(things_.data.get());
    for (auto i = 0; i < num_things(); i++, things++) {
        things->x     = Common::swap16(things->x);
        things->y     = Common::swap16(things->y);
        things->angle = Common::swap16(things->angle);
        things->type  = Common::swap16(things->type);
        things->flags = Common::swap16(things->flags);
    }

    auto linedefs = reinterpret_cast<LineDef*>(linedefs_.data.get());
    for (auto i = 0; i < num_linedefs(); i++, linedefs++) {
        linedefs->start      = Common::swap16(linedefs->start);
        linedefs->end        = Common::swap16(linedefs->end);
        linedefs->flags      = Common::swap16(linedefs->flags);
        linedefs->type       = Common::swap16(linedefs->type);
        linedefs->tag        = Common::swap16(linedefs->tag);
        linedefs->sidedef[0] = Common::swap16(linedefs->sidedef[0]);
        linedefs->sidedef[1] = Common::swap16(linedefs->sidedef[1]);
    }

    auto sidedefs = reinterpret_cast<SideDef*>(sidedefs_.data.get());
    for (auto i = 0; i < num_sidedefs(); i++, sidedefs++) {
        sidedefs->x      = Common::swap16(sidedefs->x);
        sidedefs->y      = Common::swap16(sidedefs->y);
        sidedefs->sector = Common::swap16(sidedefs->sector);
    }

    auto vertices = reinterpret_cast<Vertex*>(vertices_.data.get());
    for (auto i = 0; i < num_vertices(); i++, vertices++) {
        vertices->x = Common::swap16(vertices->x);
        vertices->y = Common::swap16(vertices->y);
    }

    auto segs = reinterpret_cast<Seg*>(segs_.data.get());
    for (auto i = 0; i < num_segs(); i++, segs++) {
        segs->start   = Common::swap16(segs->start);
        segs->end     = Common::swap16(segs->end);
        segs->angle   = Common::swap16(segs->angle);
        segs->linedef = Common::swap16(segs->linedef);
        segs->dir     = Common::swap16(segs->dir);
        segs->offset  = Common::swap16(segs->offset);
    }

    auto ssectors = reinterpret_cast<SSector*>(ssectors_.data.get());
    for (auto i = 0; i < num_ssectors(); i++, ssectors++) {
        ssectors->count = Common::swap16(ssectors->count);
        ssectors->first = Common::swap16(ssectors->first);
    }

    auto nodes = reinterpret_cast<Node*>(nodes_.data.get());
    for (auto i = 0; i < num_nodes(); i++, nodes++) {
        nodes->x        = Common::swap16(nodes->x);
        nodes->y        = Common::swap16(nodes->y);
        nodes->dx       = Common::swap16(nodes->dx);
        nodes->dy       = Common::swap16(nodes->dy);
        nodes->child[0] = Common::swap16(nodes->child[0]);
        nodes->child[1] = Common::swap16(nodes->child[1]);

        for (int j = 0; j < 4; j++) {
            nodes->lbounds[j] = Common::swap16(nodes->lbounds[j]);
            nodes->rbounds[j] = Common::swap16(nodes->rbounds[j]);
        }
    }

    auto sectors = reinterpret_cast<Sector*>(sectors_.data.get());
    for (auto i = 0; i < num_sectors(); i++, sectors++) {
        sectors->floorh  = Common::swap16(sectors->floorh);
        sectors->ceilh   = Common::swap16(sectors->ceilh);
        sectors->light   = Common::swap16(sectors->light);
        sectors->special = Common::swap16(sectors->special);
        sectors->tag     = Common::swap16(sectors->tag);
    }

    auto reject = reinterpret_cast<Reject*>(reject_.data.get());
    for (auto i = 0; i < num_reject(); i++, reject++) {
        // TODO
    }

    auto blockmap = reinterpret_cast<BlockMap*>(blockmap_.data.get());
    for (auto i = 0; i < num_blockmap(); i++, blockmap++) {
        blockmap->x    = Common::swap16(blockmap->x);
        blockmap->y    = Common::swap16(blockmap->y);
        blockmap->cols = Common::swap16(blockmap->cols);
        blockmap->rows = Common::swap16(blockmap->rows);
    }
#endif
}
