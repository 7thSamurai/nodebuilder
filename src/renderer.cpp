#include "renderer.hpp"
#include "map.hpp"
#include "SDL2/SDL.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <climits>

Renderer::Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map) : map_(map), width_(width), height_(height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL");

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
        throw std::runtime_error("Error creating SDL window");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
        throw std::runtime_error("Error creating SDL renderer");
}

Renderer::~Renderer() {
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}

void Renderer::draw_map() {
    auto linedefs = map_.get_linedefs();
    auto vertices = map_.get_vertices();

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);

    for (auto i = 0; i < map_.num_linedefs(); i++) {
        auto p1 = convert(Vec2f(vertices[linedefs[i].start].x, vertices[linedefs[i].start].y));
        auto p2 = convert(Vec2f(vertices[linedefs[i].end].x, vertices[linedefs[i].end].y));

        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }

    for (const auto &pair : polys)
        draw_poly(pair.first, pair.second);
}

void Renderer::draw_line(const Linef &line) {
    auto p1 = convert(line.a);
    auto p2 = convert(line.b);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

void Renderer::draw_box(const Boxf &box) {
    //return;
    auto pos  = convert(box.min());
    auto size = convert(Vec2f(map_.offset().x, map_.offset().y) + box.max() - box.min());

    SDL_Rect rect = {
        static_cast<int>(pos.x),
        static_cast<int>(pos.y),
        static_cast<int>(size.x),
        static_cast<int>(size.y)
    };

    SDL_SetRenderDrawColor(renderer, 0x80, 0x00, 0x80, 0xff);
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::draw_poly(const Polyf &poly, const Color &color) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

    // Find the minimum and maximum Y coords
    int ymin = poly.at(0).y;
    int ymax = poly.at(0).y;

    for (auto i = 1; i < poly.size(); i++) {
        int y = poly.at(i).y;

        if (y < ymin)
            ymin = y;
        if (y > ymax)
            ymax = y;
    }

    // Convert the Y coordinates to screen space
    ymin = converty(ymin);
    ymax = converty(ymax);

    int xmin[ymax - ymin + 1];
    int xmax[ymax - ymin + 1];

    std::fill_n(xmin, ymax - ymin + 1, INT_MAX);
    std::fill_n(xmax, ymax - ymin + 1, INT_MIN);

    // Find the minium and maximum X values of each scanline
    for (auto i = 0; i < poly.size(); i++) {
        int x0 = convertx(poly.at(i+0).x);
        int y0 = converty(poly.at(i+0).y);

        int x1 = convertx(poly.at(i+1).x);
        int y1 = converty(poly.at(i+1).y);

        bool steep = false;
        if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
            std::swap(x0, y0);
            std::swap(x1, y1);
            steep = true;
        }
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        int dx = x1 - x0;
        int dy = y1 - y0;
        float derror2 = std::abs(dy) * 2;
        float error = 0;
        int y = y0;

        for (int x = x0; x <= x1; x++) {
            if (steep) {
                if (y < xmin[x - ymin]) xmin[x - ymin] = y;
                if (y > xmax[x - ymin]) xmax[x - ymin] = y;
            }
            else {
                if (x < xmin[y - ymin]) xmin[y - ymin] = x;
                if (x > xmax[y - ymin]) xmax[y - ymin] = x;
            }

            error += derror2;
            if (error > dx) {
                y += (y1 > y0 ? 1 : -1);
                error -= dx*2;
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xff);

    // Rasterize each scanline
    for (int y = ymin; y <= ymax; y++)
        SDL_RenderDrawLine(renderer, xmin[y - ymin], y, xmax[y - ymin], y);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);

    for (auto i = 0; i < poly.size(); i++) {
        auto p1 = convert(poly.at(i+0));
        auto p2 = convert(poly.at(i+1));
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }
}

void Renderer::draw_splitter(const Splitter &splitter) {
    // TODO
    auto p = convert(splitter.p);

    // Calculate the rise and the run of the splitter
    float dist = std::sqrt(splitter.dx*splitter.dx + splitter.dy*splitter.dy);
    float run  = splitter.dx / dist;
    float rise = splitter.dy / dist;

    dist = std::max(map_.size().x, map_.size().y);

    // Extend the splitter to reach both ends of the screen
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, p.x - run*dist, p.y - rise*dist, p.x + run*dist, p.y + rise*dist);
    SDL_RenderDrawLine(renderer, p.x - run*dist - 1, p.y - rise*dist, p.x + run*dist - 1, p.y + rise*dist);
    SDL_RenderDrawLine(renderer, p.x - run*dist + 1, p.y - rise*dist, p.x + run*dist + 1, p.y + rise*dist);

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, p.x, p.y, p.x + splitter.dx, p.y + splitter.dy);
}

void Renderer::add_poly(const Polyf &poly, const Color &color) {
    polys.push_back(std::make_pair(poly, color));
}

void Renderer::show() {
    SDL_RenderPresent(renderer);
    SDL_Delay(10); // TODO: Remove
}

float Renderer::convertx(float x) const {
    return (x - map_.offset().x) / map_.size().x * width_;
}

float Renderer::converty(float y) const {
    return (y - map_.offset().y) / map_.size().y * height_;
}

Vec2f Renderer::convert(const Vec2f &p) const {
    return Vec2f(
        convertx(p.x),
        converty(p.y)
    );
}
