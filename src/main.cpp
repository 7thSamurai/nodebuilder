#include <iostream>
#include "wad.hpp"
#include "map.hpp"
#include "renderer.hpp"
#include "bsp.hpp"
#include "SDL2/SDL.h"

int main(int argc, char **argv) {
    /*Splitter splitter(Seg(Vec2f(-10, 0), Vec2f(10, 0), 0, 0, 0));
    splitter.side_of(Vec2f(0, 5));

    return 0;*/

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [WAD PATH]" << std::endl;
        return 1;
    }

    try {
        Wad wad(argv[1]);
        Map map("E1M1", wad);

        if (!map.load()) {
            std::cerr << "Error loading map!" << std::endl;
            return 1;
        }

        Renderer renderer("Node Builder", 1024, 768, map);
        renderer.draw_map();
        renderer.show();

        Bsp bsp(map);
        bsp.build(renderer);

        renderer.draw_map();
        renderer.show();

        bool running = true;
        SDL_Event event;

        while (running) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT)
                    running = false;
            }
        }
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
