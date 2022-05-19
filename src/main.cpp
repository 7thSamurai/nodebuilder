#include <iostream>
#include <chrono>

#include "wad.hpp"
#include "map.hpp"
#include "renderer.hpp"
#include "bsp.hpp"

#define VERSION "0.99"

const char *banner = "NodeBuilder - Version " VERSION " (C) 2022 Zach Collins\n";

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [WAD PATH] [MAPS...] [OPTIONS...]" << std::endl;
        return 1;
    }

    std::cout << banner << std::endl;

    std::vector<std::string> maps;
    bool draw = false;

    for (int i = 2; i < argc; i++) {
        auto arg = std::string(argv[i]);

        if (arg == "--draw")
            draw = true;
        else
            maps.push_back(argv[i]);
    }

    try {
        Wad wad(argv[1]);

        // If no maps were supplied, do them all
        if (maps.empty()) {
            maps = wad.maps();

            // If no valid maps were found
            if (maps.empty()) {
                std::cerr << "Unable to find any maps in WAD!" << std::endl;
                return 1;
            }
        }

        std::cout << "Processing " << maps.size() << " maps..." << std::endl;

        std::chrono::milliseconds total_time(0);

        for (const auto &name : maps) {
            std::cout << "Processing " << name << "...\t" << std::flush;

            auto map_time_start = std::chrono::high_resolution_clock::now();
            Map map(name, wad);

            if (!map.load()) {
                std::cerr << "\nError loading map " << name << std::endl;
                return 1;
            }

            std::string title;
            if (draw)
                title = "Node Builder - " + name;

            Renderer renderer(title, 1024, 768, map);
            renderer.draw_map();
            renderer.show();

            Bsp bsp(map);
            bsp.build(renderer);

            if (!renderer.running()) {
                std::cout << "\nKilled" << std::endl;
                return 1;
            }

            renderer.draw_map();
            renderer.show();

            auto map_time_end = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::milliseconds>(map_time_end - map_time_start);

            if (draw) {
                auto dur = std::chrono::duration_cast<std::chrono::seconds>(map_time_end - map_time_start);
                std::cout << dur.count() << "\tsec" << std::endl;
            }
            else {
                auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(map_time_end - map_time_start);
                std::cout << dur.count() << "\tms" << std::endl;
            }

            while (draw) {
                renderer.draw_map();
                renderer.show();

                if (!renderer.running())
                    break;
            }
        }

        if (maps.size() > 1)
            std::cout << "\nAll maps proccessed in " << total_time.count() << " ms" << std::endl;

        std::cout << "Saving to WAD..." << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
