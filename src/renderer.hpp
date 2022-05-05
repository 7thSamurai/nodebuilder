#pragma once

#include <string>
#include <functional>

#include "line.hpp"
#include "box.hpp"
#include "polygon.hpp"
#include "splitter.hpp"
#include "color.hpp"

struct SDL_Window;
struct SDL_Renderer;

class Map;

class Renderer
{
public:
    Renderer();
    Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map);
    ~Renderer();

    void draw_map();
    void draw_line(const Linef &line);
    void draw_box(const Boxf &box);
    void draw_poly(const Polyf &poly);
    void draw_filled_poly(const Polyf &poly, const Color &color);
    void draw_splitter(const Splitter &splitter);

    void add_poly(const Polyf &poly, const Color &color);

    void show();

private:
    void draw_line(float x0, float y0, float x1, float y1, const Color &color, const std::function<void(int x, int y, float brightess)> &plot);
    void draw_line(float x0, float y0, float x1, float y1, const Color &color);

    float convertx(float x) const;
    float converty(float y) const;
    Vec2f convert(const Vec2f &p) const;

    SDL_Window *window;
    SDL_Renderer *renderer;

    Map &map_;
    unsigned int width_, height_;

    std::vector<std::pair<Polyf, Color>> polys;
};
