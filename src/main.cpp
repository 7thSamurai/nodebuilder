#include <iostream>
#include "wad.hpp"
#include "map.hpp"
#include "renderer.hpp"
#include "bsp.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [WAD PATH]" << std::endl;
        return 1;
    }

    try {
        Wad wad(argv[1]);
        Map map("E1M2", wad);

        if (!map.load()) {
            std::cerr << "Error loading map!" << std::endl;
            return 1;
        }

        Renderer renderer("Node Builder", 1024, 768, map);
        renderer.draw_map();
        renderer.show();

        Bsp bsp(map);
        bsp.build(renderer);

        renderer.show();
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
