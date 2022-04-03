#pragma once

#include <string>
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

    void *read(const std::string &name, std::size_t &size);
    void write(const std::string &name, const void *data, std::size_t size);

    void *read_map_lump(const std::string &map, const std::string &name, std::size_t &size);
    void write_map_lump(const std::string &map, const std::string &name, const void *data, std::size_t size);

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

    struct LumpInfo {
        Lump lump;
        std::uint8_t *new_data;
        std::uint8_t *cache_data;
    };

    void *read(LumpInfo *lump, std::size_t &size);
    void write(LumpInfo *lump, const void *data, std::size_t size);

    LumpInfo *find_lump(const std::string &name);
    LumpInfo *find_map_lump(const std::string &map, const std::string &name);
    
    std::size_t lump_index(const LumpInfo *lump);
    bool is_map(const char *name);
    void find_maps();

    Header header;
    std::vector<LumpInfo> lumps;

    std::ifstream file;
    std::string path_;
    bool changed;
    
    LumpInfo *map_start, *map_end;
};
