// Copyright (C) 2022 Zach Collins <zcollins4@proton.me>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <string>
#include <functional>

#include "line.hpp"
#include "box.hpp"
#include "polygon.hpp"
#include "splitter.hpp"
#include "color.hpp"

#include "SDL2/SDL.h"
#include "cairo.h"

class Map;

class Renderer
{
public:
    Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map);
    ~Renderer();

    void draw_map();
    void draw_map_outline();
    void draw_line(const Linef &line);
    void draw_line(const Linef &line, const Color &color);
    void draw_box(const Boxf &box);
    void draw_poly(const Polyf &poly);
    void draw_filled_poly(Polyf poly, const Color &color);
    void draw_splitter(const Splitter &splitter);
    void draw_text(const std::string &text);

    void add_poly(const Polyf &poly, const Color &color);
    void add_line(const Linef &line, const Color &color);

    void clear();
    void show();

    bool running();
    bool drawing() const;

private:
    void draw_line(float x0, float y0, float x1, float y1, const Color &color, const std::function<void(int x, int y, float brightess)> &plot);
    void draw_line(float x0, float y0, float x1, float y1, const Color &color);

    void calc_aspect(int width, int height);

    float convertx(float x) const;
    float converty(float y) const;
    Vec2f convert(const Vec2f &p) const;

    // SDL stuff
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;

    // Cairo stuff
    cairo_t *cairo_context;
    cairo_surface_t *cairo_surface;

    Map &map_;
    unsigned int width_, height_;
    unsigned int offsetx_, offsety_;
    bool running_;

    std::vector<std::pair<Polyf, Color>> polys;
    std::vector<std::pair<Linef, Color>> lines;
};
