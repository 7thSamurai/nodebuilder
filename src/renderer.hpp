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
    Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map);
    ~Renderer();

    void draw_map();
    void draw_line(const Linef &line);
    void draw_box(const Boxf &box);
    void draw_splitter(const Splitter &splitter);

    void show();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Map &map_;
    Vec2i offset; // Map start offset
    Vec2i size;   // Map size
};
