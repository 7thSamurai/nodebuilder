#include "wad.hpp"
#include "common.hpp"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <cstring>

Wad::Wad(const std::string &path) : path_(path), changed(false), map_start(nullptr), map_end(nullptr) {
    file.open(path, std::ios::in | std::ios::binary);
    if (!file.good())
        throw std::runtime_error("Unable to open file " + path);

    // Read the header
    file.read(reinterpret_cast<char*>(&header), sizeof(Header));

    // Make sure that this file is a WAD
    if (std::string(header.id, 4) != "IWAD" && std::string(header.id, 4) != "PWAD")
        throw std::runtime_error("File " + path + " is not a WAD");

    lumps.reserve(header.num_lumps);
    file.seekg(header.dir_offset, std::ios::beg);

    // Read the lumps
    for (auto i = 0; i < header.num_lumps; i++) {
        LumpInfo lump;

        // Read and covert the data
        file.read(reinterpret_cast<char*>(&lump.lump), sizeof(Lump));
        lump.lump.pos  = Common::little32(lump.lump.pos);
        lump.lump.size = Common::little32(lump.lump.size);

        lumps.push_back(std::move(lump));
    }

    find_maps();
}

Wad::~Wad() {
    file.close();
}

bool Wad::save(const std::string &new_path) {
    std::string temp_path = new_path;

    if (new_path == path_) {
        if (!changed)
            return true;

        temp_path = path_ + ".XXXXXX";
    }

    // Open the output file
    std::ofstream temp(temp_path, std::ios::out | std::ios::binary);
    if (!temp.good())
        return false;

    temp.seekp(sizeof(Header), std::ios::beg);

    // Write all the lump data
    for (auto &lump : lumps) {
    	// Save the position of the data for later
        lump.lump.pos = temp.tellp();

        // Skip any virtual lumps
        if (!lump.lump.size)
            continue;

        if (lump.new_data) {
            temp.write(reinterpret_cast<const char*>(lump.new_data.get()), lump.lump.size);
        }
        else if (lump.cache_data) {
            temp.write(reinterpret_cast<const char*>(lump.cache_data.get()), lump.lump.size);
        }
        else {
            auto buffer = std::make_unique<char[]>(lump.lump.size);
            file.seekg(lump.lump.pos);
            file.read(buffer.get(), lump.lump.size);
            temp.write(buffer.get(), lump.lump.size);
        }
    }

    header.num_lumps  = lumps.size();
    header.dir_offset = temp.tellp();

    // Write the directory data
    for (const auto &lump : lumps) {
        // Convert the data
        Lump l = lump.lump;
        l.pos  = Common::little32(l.pos);
        l.size = Common::little32(l.size);

        temp.write(reinterpret_cast<char*>(&l), sizeof(Lump));
    }

    // Write the header
    temp.seekp(0, std::ios::beg);
    temp.write(reinterpret_cast<char*>(&header), sizeof(Header));
    temp.close();

    // Get rid of temporary file
    if (new_path == path_) {
        file.close();

        // Delete the old file, and replace with the new one
        std::filesystem::remove(std::filesystem::path(path_));
        std::filesystem::rename(std::filesystem::path(temp_path), std::filesystem::path(path_));

        // Reopen the file
        file.open(path_, std::ios::in | std::ios::binary);
        if (!file.good())
            return false;
    }

    // Clear out any of the new data
    for (auto &lump : lumps) {
        if (lump.new_data) {
 			lump.cache_data.reset(nullptr);
            lump.cache_data.swap(lump.new_data);
        }
    }

    changed = false;

    return true;
}

std::string Wad::name() const {
    return path_;
}

bool Wad::has_changed() const {
    return changed;
}

std::uint32_t Wad::file_size() const {
    std::uint32_t size = 0;
    for (const auto &lump : lumps)
        size += lump.lump.size;

    return size;
}

std::vector<std::string> Wad::maps() const {
    std::vector<std::string> maps;

    // Find the names of all the maps
    for (LumpInfo *lump = map_start; lump <= map_end; lump++) {
        if (is_map(lump->lump.name))
            maps.push_back(std::string(lump->lump.name));
    }

    return maps;
}

std::unique_ptr<std::uint8_t[]> Wad::read(const std::string &name, std::size_t &size) {
    auto lump = find_lump(name);
    if (!lump) {
        size = 0;
        return nullptr;
    }

    return read(lump, size);
}

bool Wad::write(const std::string &name, const void *data, std::size_t size) {
    auto lump = find_lump(name);
    if (!lump)
        return false;

    write(lump, data, size);

    return true;
}

bool Wad::insert(const std::string &after, const std::string &name, const void *data, std::size_t size) {
    // If the lump already exists just write to that
    auto lump = find_lump(name);
    if (lump) {
        write(lump, data, size);
        return true;
    }

    lump = find_lump(after);
    if (!lump)
        return false;

    insert(lump, name, data, size);

    return true;
}

std::unique_ptr<std::uint8_t[]> Wad::read_map_lump(const std::string &map, const std::string &name, std::size_t &size) {
    auto lump = find_map_lump(map, name);
    if (!lump) {
        size = 0;
        return nullptr;
    }

    return read(lump, size);
}

bool Wad::write_map_lump(const std::string &map, const std::string &name, const void *data, std::size_t size) {
    auto lump = find_map_lump(map, name);
    if (!lump)
        return false;

    write(lump, data, size);

    return true;
}

bool Wad::insert_map_lump(const std::string &map, const std::string &after, const std::string &name, const void *data, std::size_t size) {
    // If the lump already exists just write to that
    auto lump = find_map_lump(map, name);
    if (lump) {
        write(lump, data, size);
        return true;
    }

    lump = find_map_lump(map, after);
    if (!lump)
        return false;

    insert(lump, name, data, size);

    return true;
}

void Wad::remove(const std::string &name) {
    auto lump = find_lump(name);
    if (!lump)
        return;

	lump->new_data.reset(nullptr);
	lump->cache_data.reset(nullptr);

    lumps.erase(lumps.begin() + lump_index(lump));
    changed = true;
    find_maps();
}

std::unique_ptr<std::uint8_t[]> Wad::read(LumpInfo *lump, std::size_t &size) {
    auto buffer = std::make_unique<std::uint8_t[]>(lump->lump.size);

    if (lump->new_data) {
        std::copy_n(lump->new_data.get(), lump->lump.size, buffer.get());
    }
    else if (lump->cache_data) {
        std::copy_n(lump->cache_data.get(), lump->lump.size, buffer.get());
    }
    else {
        file.seekg(lump->lump.pos);
        file.read(reinterpret_cast<char*>(buffer.get()), lump->lump.size);

        // Cache the data
        lump->cache_data = std::make_unique<std::uint8_t[]>(lump->lump.size);
        std::copy_n(buffer.get(), lump->lump.size, lump->cache_data.get());
    }

    size = lump->lump.size;

    return buffer;
}

void Wad::write(LumpInfo *lump, const void *data, std::size_t size) {
    // Delete any of the old stuff
	lump->new_data.reset(nullptr);
	lump->cache_data.reset(nullptr);

    // Copy the new data over
    lump->new_data = std::make_unique<std::uint8_t[]>(size);
    std::copy_n(reinterpret_cast<const std::uint8_t*>(data), size, lump->new_data.get());
    lump->lump.size = size;

    changed = true;
}

void Wad::insert(LumpInfo *after, const std::string &name, const void *data, std::size_t size) {
    LumpInfo lump;

    std::fill_n(lump.lump.name, 8, 0);
    std::copy_n(&name[0], std::min<std::size_t>(name.size(), 8), lump.lump.name);

    lumps.insert(lumps.begin() + lump_index(after), std::move(lump));
    write(after + 1, data, size);
}

Wad::LumpInfo *Wad::find_lump(const std::string &name) {
    // Look for a particular lump
    for (auto &lump : lumps) {
        if (lump.lump.name[0] == name[0]) {
            if (!strncmp(lump.lump.name, &name[0], 8))
                return &lump;
        }
    }

    return nullptr;
}

Wad::LumpInfo *Wad::find_map_lump(const std::string &map, const std::string &name) {
    if (!map_start || !map_end)
        return nullptr;

    LumpInfo *start = nullptr;
    LumpInfo *end   = nullptr;

    // Find the map marker
    for (LumpInfo *lump = map_start; lump <= map_end; lump++) {
        if (lump->lump.name[0] == map[0]) {
            if (!strncmp(lump->lump.name, &map[0], 8)) {
                start = lump;
                break;
            }
        }
    }

    if (!start)
        return nullptr;

    // Find the end of the map's lumps
    end = start + (&lumps.back()-end-10 >= 0 ? 10 : &lumps.back()-end);

    // Now find the actual lump
    for (LumpInfo *lump = start+1; lump <= end; lump++) {
        // Don't take from another map's lumps
        if (is_map(lump->lump.name))
            return nullptr;

        if (lump->lump.name[0] == name[0]) {
            if (!strncmp(lump->lump.name, &name[0], 8))
                return lump;
        }
    }

    return nullptr;
}

std::size_t Wad::lump_index(const LumpInfo *lump) {
    return lump - &lumps[0];
}

bool Wad::is_map(const char *name) const {
    if (name[0] == 'M') {
        if (name[1] == 'A' && name[2] == 'P') {
            if (!std::isdigit(name[3])) return false;
            if (!std::isdigit(name[4])) return false;
            if (name[5]) return false;

            return true;
        }

        return false;
    }

    if (name[0] == 'E') {
        if (name[2] == 'M') {
            if (!std::isdigit(name[1])) return false;
            if (!std::isdigit(name[3])) return false;
            if (name[4]) return false;

            return true;
        }

        return false;
   }

   return false;
}

void Wad::find_maps() {
    map_start = map_end = nullptr;

    // Find the first map
    std::size_t first;
    for (first = 0; first < lumps.size(); first++) {
        if (is_map(lumps[first].lump.name)) {
            map_start = &lumps[first];
            break;
        }
    }

    // Find the last map
    for (std::size_t end = lumps.size() - 1; end >= first; end--) {
        if (is_map(lumps[first].lump.name)) {
            map_end = &lumps[end] + (lumps.size()-end-11 >= 0 ? 10 : lumps.size()-end-1); // Add the correct offset without overflowing
            break;
        }
    }
}
