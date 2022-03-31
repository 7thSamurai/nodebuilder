#include <iostream>
#include "wad.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [WAD PATH]" << std::endl;
        return 1;
    }

    try {
        Wad wad(argv[1]);
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
