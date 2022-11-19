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

#include "renderer.hpp"
#include "map.hpp"

#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <climits>

Renderer::Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map) : map_(map), running_(true) {
    if (name.empty()) {
        window   = nullptr;
        renderer = nullptr;
        return;
    }

    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL");

    // Create a window
    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
        throw std::runtime_error("Error creating SDL window");

    // Setup the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        throw std::runtime_error("Error creating SDL renderer");

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Create the SDL texture
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture)
        throw std::runtime_error("Error creating SDL texture");

    // Setup the Cairo context
    cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_context = cairo_create(cairo_surface);

    // Fix the aspect ratio
    calc_aspect(width, height);

    // Setup the transformation matrix
    // TODO
}

Renderer::~Renderer() {
    // Clean up Cairo
    if (cairo_context)
        cairo_destroy(cairo_context);
    if (cairo_surface)
        cairo_surface_destroy(cairo_surface);

    // Clean up SDL
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

void Renderer::draw_map() {
    if (!drawing())
        return;

    draw_map_outline();

    for (const auto& [poly, color] : polys)
        draw_filled_poly(poly, color);

    for (const auto& [poly, color] : polys) {
        for (auto i = 0; i < poly.size(); i++) {
            auto p1 = convert(poly.at(i+0));
            auto p2 = convert(poly.at(i+1));

            draw_line(p1.x, p1.y, p2.x, p2.y, Color(0x3d, 0x3d, 0x3d));
        }
    }
}

void Renderer::draw_map_outline() {
    if (!drawing())
        return;

    auto linedefs = map_.get_linedefs();
    auto vertices = map_.get_vertices();

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);

    for (auto i = 0; i < map_.num_linedefs(); i++) {
        auto p1 = convert(Vec2f(vertices[linedefs[i].start].x, vertices[linedefs[i].start].y));
        auto p2 = convert(Vec2f(vertices[linedefs[i].end].x, vertices[linedefs[i].end].y));

        draw_line(p1.x, p1.y, p2.x, p2.y, Color(0x3d, 0x3d, 0x3d));
    }

    for (const auto& [line, color] : lines) {
        auto p1 = convert(line.a);
        auto p2 = convert(line.b);

        draw_line(p1.x, p1.y, p2.x, p2.y, color);
    }
}

void Renderer::draw_line(const Linef &line) {
    if (!drawing())
        return;

    auto p1 = convert(line.a);
    auto p2 = convert(line.b);

    draw_line(p1.x, p1.y, p2.x, p2.y, Color(0xff, 0xff, 0xff));
}

void Renderer::draw_line(const Linef &line, const Color &color) {
    if (!drawing())
        return;

    auto p1 = convert(line.a);
    auto p2 = convert(line.b);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::draw_box(const Boxf &box) {
    if (!drawing())
        return;

    auto pos  = convert(box.min());
    auto size = convert(box.max()) - convert(box.min());

    SDL_Rect rect = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };

    SDL_SetRenderDrawColor(renderer, 0x80, 0x00, 0x80, 0xff);
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::draw_poly(const Polyf &poly) {
    if (!drawing())
        return;

    for (auto i = 0; i < poly.size(); i++) {
        auto p1 = convert(poly.at(i+0));
        auto p2 = convert(poly.at(i+1));
        draw_line(p1.x, p1.y, p2.x, p2.y, Color(0x80, 0x00, 0x80));
    }
}

// Warning: Extremelly hacky code (I am not really sure on the best way to do this, so I tossed this together real quick...)
void Renderer::draw_filled_poly(Polyf poly, const Color &color) {
    if (!drawing())
        return;

    // Convert all the points to screen space
    for (auto i = 0; i < poly.size(); i++)
        poly[i] = convert(poly[i]);

    // Find the bounds of the polygon and convert them to screen space
    auto bounds = poly.bounds();
    bounds = Boxf(bounds.min(), bounds.max());

    const int total_height = height_ + offsety_;

    int xmin[total_height];
    int xmax[total_height];

    int linex_all[total_height]; // All X points
    int linex[total_height];     // Anti-aliased X points

    std::fill_n(xmin, sizeof(xmin)/sizeof(int), INT_MAX);
    std::fill_n(xmax, sizeof(xmax)/sizeof(int), INT_MIN);

    float slope;
    bool left_edge;

    // Find the minimum and maximum X coords of the line
    // depending on if it is a left or right edge
    auto plot = [&](int x, int y, float brightess) {
        bool transparent = slope < 0.5f && brightess < 0.5f;

        if (left_edge) {
            if (x > linex[y]) {
                linex_all[y] = x;
                if (!transparent) linex[y] = x;
            }
        }
        else {
            if (x < linex[y]) {
                linex_all[y] = x;
                if (!transparent) linex[y] = x;
            }
        }
    };

    // Loop through each edge
    for (auto i = 0; i < poly.size(); i++) {
        float x0 = poly.at(i+0).x;
        float y0 = poly.at(i+0).y;

        float x1 = poly.at(i+1).x;
        float y1 = poly.at(i+1).y;

        // Find the slope of the line
        float dx = x1 - x0;
        float dy = y1 - y0;
        slope = std::abs(!dx ? 1.0f : dy / dx);

        // Figure out if this edge is on the left or right
        auto p = poly.at(i+0) + (poly.at(i+1) - poly.at(i+0))/2 + Vec2f(1, 0);
        left_edge = poly.point_inside(p);

        std::fill_n(linex_all, sizeof(linex_all)/sizeof(int), left_edge ? INT_MIN : INT_MAX);
        std::fill_n(linex, sizeof(linex)/sizeof(int), left_edge ? INT_MIN : INT_MAX);

        draw_line(x0, y0,  x1, y1, Color(), plot);

        auto valid = [](int val) -> bool {
            return val != INT_MIN && val != INT_MAX;
        };

        // Find the minimum and maximum x coords for all of the edges
        for (int y = std::min(y0, y1); y <= std::max(y0, y1); y++) {
            int val;
            if (valid(linex[y]))
                val = linex[y];
            else if (valid(linex_all[y]))
                val = linex_all[y];
            else
                continue;

            if (val < xmin[y]) xmin[y] = val;
            if (val > xmax[y]) xmax[y] = val;
        }
    }

    // Rasterize each scanline
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);
    for (int y = bounds.min().y+1; y <= bounds.max().y; y++)
        SDL_RenderDrawLine(renderer, xmin[y], y, xmax[y], y);
}

void Renderer::draw_splitter(const Splitter &splitter) {
    if (!drawing())
        return;

    // TODO
    auto p = convert(splitter.p);

    // Calculate the rise and the run of the splitter
    float dist = std::sqrt(splitter.dx*splitter.dx + splitter.dy*splitter.dy);
    float run  = splitter.dx / dist;
    float rise = splitter.dy / dist;

    dist = std::max(map_.size().x, map_.size().y);

    // Extend the splitter to reach both ends of the screen
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0x00, 0xff);
    draw_line(p.x - run*dist, p.y - rise*dist, p.x + run*dist, p.y + rise*dist, Color(0xff, 0xff, 0x00));
    draw_line(p.x - run*dist - 1, p.y - rise*dist, p.x + run*dist - 1, p.y + rise*dist, Color(0xff, 0xff, 0x00));
    draw_line(p.x - run*dist + 1, p.y - rise*dist, p.x + run*dist + 1, p.y + rise*dist, Color(0xff, 0xff, 0x00));
}

void Renderer::add_poly(const Polyf &poly, const Color &color) {
    if (!drawing())
        return;

    polys.push_back(std::make_pair(poly, color));
}

void Renderer::add_line(const Linef &line, const Color &color) {
    if (!drawing())
        return;

    lines.push_back(std::make_pair(line, color));
}

void Renderer::show() {
    if (!drawing())
        return;

    SDL_RenderPresent(renderer);
    SDL_Delay(10); // TODO: Remove
}

bool Renderer::running() {
    if (!drawing())
        return running_;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running_ = false;

        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running_ = false;
        }
    }

    return running_;
}

bool Renderer::drawing() const {
    return window && renderer;
}

void Renderer::draw_line(float x0, float y0, float x1, float y1, const Color &color, const std::function<void(int x, int y, float brightess)> &plot) {
    // Xiaolin Wu's line algorithm
    auto ipart  = [](float x) -> int { return static_cast<int>(x); };
    auto round  = [](float x) -> float { return std::round(x); };
    auto fpart  = [&](float x) -> float { return x - ipart(x); };
    auto rfpart = [&](float x) -> float { return 1.0f - fpart(x); };

    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = !dx ? 1.0f : dy / dx;

    // Handle first endpoint
    float xend = round(x0);
    float yend = y0 + gradient * (xend - x0);
    float xgap = rfpart(x0 + 0.5f);
    int xpxl1 = xend;
    int ypxl1 = ipart(yend);

    if (steep) {
        plot(ypxl1,   xpxl1, rfpart(yend) * xgap);
        plot(ypxl1+1, xpxl1,  fpart(yend) * xgap);
    }
    else {
        plot(xpxl1, ypxl1  , rfpart(yend) * xgap);
        plot(xpxl1, ypxl1+1,  fpart(yend) * xgap);
    }
    float intery = yend + gradient;

    // Handle second endpoint
    xend = round(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = fpart(x1 + 0.5f);
    int xpxl2 = xend;
    int ypxl2 = ipart(yend);

    if (steep) {
        plot(ypxl2  , xpxl2, rfpart(yend) * xgap);
        plot(ypxl2+1, xpxl2,  fpart(yend) * xgap);
    }
    else {
        plot(xpxl2, ypxl2,   rfpart(yend) * xgap);
        plot(xpxl2, ypxl2+1, fpart(yend) * xgap);
    }

    // Main loop
    if (steep) {
        for (int x = xpxl1 + 1; x < xpxl2; x++) {
            plot(ipart(intery),   x, rfpart(intery));
            plot(ipart(intery)+1, x, fpart(intery));
            intery = intery + gradient;
        }
    }
    else {
        for (int x = xpxl1 + 1; x < xpxl2; x++) {
            plot(x, ipart(intery),   rfpart(intery));
            plot(x, ipart(intery)+1, fpart(intery));
            intery = intery + gradient;
       }
    }
}

void Renderer::draw_line(float x0, float y0, float x1, float y1, const Color &color) {
    auto plot = [&](int x, int y, float brightness) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255*brightness);
        SDL_RenderDrawPoint(renderer, x, y);
    };

    draw_line(x0, y0, x1, y1, color, plot);
}

void Renderer::calc_aspect(int width, int height) {
    auto want_aspect = static_cast<float>(map_.size().x) / static_cast<float>(map_.size().y);
    auto real_aspect = static_cast<float>(width) / static_cast<float>(height);

    float scale;

    if (want_aspect > real_aspect)
        scale = static_cast<float>(width) / static_cast<float>(map_.size().x);
    else
        scale = static_cast<float>(height) / static_cast<float>(map_.size().y);

    width_  = map_.size().x * scale;
    height_ = map_.size().y * scale;

    offsetx_ = (width - width_)   / 2;
    offsety_ = (height - height_) / 2;
}

float Renderer::convertx(float x) const {
    return offsetx_ + (x - map_.offset().x) / map_.size().x * width_;
}

float Renderer::converty(float y) const {
    return offsety_ + height_- (y - map_.offset().y) / map_.size().y * height_;
}

Vec2f Renderer::convert(const Vec2f &p) const {
    return Vec2f(
        convertx(p.x),
        converty(p.y)
    );
}
