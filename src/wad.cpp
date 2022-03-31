#include "wad.hpp"
#include "common.hpp"
#include <filesystem>
#include <algorithm>
#include <iostream>

Wad::Wad(const std::string &path) : path_(path), changed(false) {
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
        lump.new_data   = nullptr;
        lump.cache_data = nullptr;

        // Read and covert the data
        file.read(reinterpret_cast<char*>(&lump.lump), sizeof(Lump));
        lump.lump.pos  = Common::little32(lump.lump.pos);
        lump.lump.size = Common::little32(lump.lump.size);

        lumps.push_back(lump);
    }
}

Wad::~Wad() {
    file.close();

    // Clean up
    for (auto &lump : lumps) {
        if (lump.new_data)
            delete[] lump.new_data;
        if (lump.cache_data)
            delete[] lump.cache_data;
    }
}

bool Wad::save(const std::string &new_path) {
    std::string temp_path = new_path;

    if (new_path == path_) {
        if (!changed)
            return true;

        temp_path = path_ + "XXXXXX";
    }

    // Open the output file
    std::ofstream temp(temp_path, std::ios::out | std::ios::binary);
    if (!temp.good())
        return false;

    temp.seekp(sizeof(Header), std::ios::beg);

    // Write all the lump data
    for (auto &lump : lumps) {
        lump.lump.pos = temp.tellp();

        // Skip any virtual lumps
        if (!lump.lump.size)
            continue;

        if (lump.new_data) {
            temp.write(reinterpret_cast<const char*>(lump.new_data), lump.lump.size);
        }
        else if (lump.cache_data) {
            temp.write(reinterpret_cast<const char*>(lump.cache_data), lump.lump.size);
        }
        else {
            auto buffer = new std::uint8_t[lump.lump.size];

            file.seekg(lump.lump.pos);
            file.read(reinterpret_cast<char*>(buffer), lump.lump.size);
            temp.write(reinterpret_cast<const char*>(buffer), lump.lump.size);

            delete[] buffer;
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
        if (!lump.new_data) {
            if (lump.cache_data)
                delete[] lump.cache_data;

            lump.cache_data = lump.new_data;
            lump.new_data   = nullptr;
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

void *Wad::read(const std::string &name, std::size_t &size) {
    auto lump = find_lump(name);
    if (!lump)
        return nullptr;

    auto buffer = new std::uint8_t[lump->lump.size];

    if (lump->new_data) {
        std::copy_n(lump->new_data, lump->lump.size, buffer);
    }
    else if (lump->cache_data) {
        std::copy_n(lump->cache_data, lump->lump.size, buffer);
    }
    else {
        file.seekg(lump->lump.pos);
        file.read(reinterpret_cast<char*>(buffer), lump->lump.size);

        // TODO: Cache the data
    }

    return static_cast<void*>(buffer);
}

void Wad::write(const std::string &name, const void *data, std::size_t size) {
    auto lump = find_lump(name);
    if (!lump) {
        // TODO
        return;
    }

    // Delete any of the old stuff
    if (lump->new_data) {
        delete[] lump->new_data;
    }
    if (lump->cache_data) {
        delete[] lump->cache_data;
        lump->cache_data = nullptr;
    }

    // Copy the new data over
    lump->new_data = new std::uint8_t[size];
    std::copy_n(reinterpret_cast<const std::uint8_t*>(data), size, lump->new_data);
    lump->lump.size = size;

    changed = true;
}

void Wad::remove(const std::string &name) {
    auto lump = find_lump(name);
    if (!lump)
        return;

    if (lump->new_data)
        delete[] lump->new_data;
    if (lump->cache_data)
        delete[] lump->cache_data;

    lumps.erase(lumps.begin() + lump_index(lump));
    changed = true;
}

Wad::LumpInfo *Wad::find_lump(const std::string &name) {
    // Look for a particular lump
    for (auto &lump : lumps) {
        if (lump.lump.name[0] == name[0]) {
            if (std::string(lump.lump.name, 8) == name)
                return &lump;
        }
    }

    return nullptr;
}

std::size_t Wad::lump_index(const LumpInfo *lump) {
    return lump - &lumps[0];
}
