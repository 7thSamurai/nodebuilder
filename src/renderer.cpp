#include "renderer.hpp"
#include "map.hpp"
#include "SDL2/SDL.h"
#include <stdexcept>
#include <iostream>

Renderer::Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map) : map_(map) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL");

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
        throw std::runtime_error("Error creating SDL window");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer)
        throw std::runtime_error("Error creating SDL renderer");

    offset = map.offset();
    size   = map.size();

    SDL_RenderSetLogicalSize(renderer, size.x, size.y);
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
        auto p1 = vertices[linedefs[i].start];
        auto p2 = vertices[linedefs[i].end];

        SDL_RenderDrawLine(renderer, p1.x - offset.x, p1.y - offset.y, p2.x - offset.x, p2.y - offset.y);
    }
}

void Renderer::draw_line(const Linef &line) {
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawLine(renderer, line.a.x - offset.x, line.a.y - offset.y, line.b.x - offset.x, line.b.y - offset.y);
}

void Renderer::draw_box(const Boxf &box) {
    SDL_Rect rect = {
        static_cast<int>(box.min().x  - offset.x),
        static_cast<int>(box.min().y  - offset.y),
        static_cast<int>(box.width()),
        static_cast<int>(box.height())
    };

    SDL_SetRenderDrawColor(renderer, 0x80, 0x00, 0x80, 0xff);
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::draw_splitter(const Splitter &splitter) {
    // Calculate the rise and the run of the splitter
    float dist = std::sqrt(splitter.dx*splitter.dx + splitter.dy*splitter.dy);
    float run  = splitter.dx / dist;
    float rise = splitter.dy / dist;

    dist = std::max(size.x, size.y);

    // Extend the splitter to reach both ends of the screen
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, splitter.p.x - run*dist - offset.x, splitter.p.y - rise*dist - offset.y, splitter.p.x + run*dist- offset.x, splitter.p.y + rise*dist - offset.y);

    SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderDrawLine(renderer, splitter.p.x - offset.x, splitter.p.y - offset.y, splitter.p.x - offset.x + splitter.dx, splitter.p.y - offset.y + splitter.dy);
}

void Renderer::show() {
    SDL_RenderPresent(renderer);
    SDL_Delay(100); // TODO: Remove
}
