#pragma once

#include <string>
#include "line.hpp"
#include "box.hpp"
#include "splitter.hpp"

struct SDL_Window;
struct SDL_Renderer;

class Map;

class Renderer
{
public:
    Renderer(const std::string &name, unsigned int width, unsigned int height);
    ~Renderer();

    void setup(const Map &map);

    void draw_map(const Map &map);
    void draw_line(const Linei &line);
    void draw_box(const Boxi &box);
    void draw_splitter(const Splitter &splitter);

    void show();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Vec2i offset; // Map start offset
    Vec2i size;   // Map size
};
