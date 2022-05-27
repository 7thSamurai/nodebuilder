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

#include <string>
#include <memory>
#include <fstream>
#include <vector>

class Wad
{
public:
    Wad(const std::string &path);
    ~Wad();

    bool save(const std::string &new_path);

    std::string name() const;
    bool has_changed() const;
    std::uint32_t file_size() const;

    std::vector<std::string> maps() const;

    std::unique_ptr<std::uint8_t[]> read(const std::string &name, std::size_t &size);
    bool write(const std::string &name, const void *data, std::size_t size);
    bool insert(const std::string &after, const std::string &name, const void *data, std::size_t size);

    std::unique_ptr<std::uint8_t[]> read_map_lump(const std::string &map, const std::string &name, std::size_t &size);
    bool write_map_lump(const std::string &map, const std::string &name, const void *data, std::size_t size);
    bool insert_map_lump(const std::string &map, const std::string &after, const std::string &name, const void *data, std::size_t size);

    void remove(const std::string &name);

private:
    struct Header {
        char id[4];
        std::int32_t num_lumps;
        std::int32_t dir_offset;
    };

    struct Lump {
        std::uint32_t pos;
        std::uint32_t size;
        char name[8];
    };

    class LumpInfo {
    public:    
        Lump lump;
        std::unique_ptr<std::uint8_t[]> new_data;
        std::unique_ptr<std::uint8_t[]> cache_data;
    };

    std::unique_ptr<std::uint8_t[]> read(LumpInfo *lump, std::size_t &size);
    void write(LumpInfo *lump, const void *data, std::size_t size);
    void insert(LumpInfo *after, const std::string &name, const void *data, std::size_t size);

    LumpInfo *find_lump(const std::string &name);
    LumpInfo *find_map_lump(const std::string &map, const std::string &name);

    std::size_t lump_index(const LumpInfo *lump);
    bool is_map(const char *name) const;
    void find_maps();

    Header header;
    std::vector<LumpInfo> lumps;

    std::ifstream file;
    std::string path_;
    bool changed;

    LumpInfo *map_start, *map_end;
};
