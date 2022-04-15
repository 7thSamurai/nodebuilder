#include "renderer.hpp"
#include "map.hpp"
#include "SDL2/SDL.h"
#include <stdexcept>
#include <iostream>

Renderer::Renderer(const std::string &name, unsigned int width, unsigned int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Error initializing SDL");

    window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window)
        throw std::runtime_error("Error creating SDL window");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
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

void Renderer::setup(const Map &map) {
    offset = map.offset();
    size   = map.size();

    SDL_RenderSetLogicalSize(renderer, size.x, size.y);
}

void Renderer::draw_map(const Map &map) {
    auto linedefs = map.get_linedefs();
    auto vertices = map.get_vertices();

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0xff);

    for (auto i = 0; i < map.num_linedefs(); i++) {
        auto p1 = vertices[linedefs[i].start];
        auto p2 = vertices[linedefs[i].end];

        SDL_RenderDrawLine(renderer, p1.x - offset.x, p1.y - offset.y, p2.x - offset.x, p2.y - offset.y);
    }
}

void Renderer::draw_line(const Linei &line) {
    SDL_RenderDrawLine(renderer, line.a.x - offset.x, line.a.y - offset.y, line.b.x - offset.x, line.b.y - offset.y);
}

void Renderer::draw_box(const Boxi &box) {
    SDL_Rect rect = {
        box.min().x - offset.x,
        box.min().y - offset.y,
        box.max().x - box.min().x - offset.x,
        box.max().y - box.min().y - offset.y
    };

    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::draw_splitter(const Splitter &splitter) {
    // Calculate the rise and the run of the splitter
    float dist = std::sqrt(splitter.dx*splitter.dx + splitter.dy*splitter.dy);
    float run  = splitter.dx / dist;
    float rise = splitter.dy / dist;

    // Extend the splitter to reach both ends of the screen
    SDL_RenderDrawLine(renderer, splitter.p.x - run*size.x, splitter.p.y - rise*size.y, splitter.p.x + run*size.x, splitter.p.y + run*size.y);
}

void Renderer::show() {
    SDL_RenderPresent(renderer);
    SDL_Delay(10000); // TODO: Remove
}
