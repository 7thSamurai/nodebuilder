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
#include <sstream>
#include <climits>

Renderer::Renderer(const std::string &name, unsigned int width, unsigned int height, Map &map) : map_(map), running_(true) {
    if (name.empty()) {
        window   = nullptr;
        renderer = nullptr;

        cairo_context = nullptr;
        cairo_surface = nullptr;

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
    cairo_set_antialias(cairo_context, CAIRO_ANTIALIAS_BEST);
    cairo_set_line_join(cairo_context, CAIRO_LINE_JOIN_BEVEL);

    // Fix the aspect ratio
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

    // Flip the Y direction
    cairo_matrix_t flip_y;
    cairo_matrix_init(&flip_y, 1, 0, 0, -1, 0, 0);
    cairo_set_matrix(cairo_context, &flip_y);
    cairo_translate(cairo_context, offsetx_, -height + offsety_);

    // Translate and scale the matrix according to the map size
    cairo_scale(cairo_context, static_cast<float>(width_) / map.size().x, static_cast<float>(height_) / map.size().y);
    cairo_translate(cairo_context, -map.offset().x, -map.offset().y);
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

    // Draw the outline
    draw_map_outline();

    // Then any polygons
    for (const auto& [poly, color] : polys)
        draw_filled_poly(poly, color);
}

void Renderer::draw_map_outline() {
    if (!drawing())
        return;

    auto linedefs = map_.get_linedefs();
    auto vertices = map_.get_vertices();

    for (auto i = 0; i < map_.num_linedefs(); i++) {
        auto p1 = Vec2f(vertices[linedefs[i].start].x, vertices[linedefs[i].start].y);
        auto p2 = Vec2f(vertices[linedefs[i].end].x, vertices[linedefs[i].end].y);

        cairo_save(cairo_context);

        cairo_move_to(cairo_context, p1.x, p1.y);
        cairo_line_to(cairo_context, p2.x, p2.y);

        cairo_set_source_rgb(cairo_context, 0.24, 0.24, 0.24);
        cairo_identity_matrix(cairo_context);
        cairo_set_line_width(cairo_context, 2);
        cairo_stroke(cairo_context);

        cairo_restore(cairo_context);
    }

    for (const auto& [line, color] : lines)
        draw_line(line, color);
}

void Renderer::draw_line(const Linef &line) {
    if (!drawing())
        return;

    cairo_save(cairo_context);

    // Draw the line
    cairo_set_source_rgb(cairo_context, 1.0, 1.0, 1.0);
    cairo_move_to(cairo_context, line.a.x, line.a.y);
    cairo_line_to(cairo_context, line.b.x, line.b.y);

    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 2);
    cairo_stroke(cairo_context);

    cairo_restore(cairo_context);
}

void Renderer::draw_line(const Linef &line, const Color &color) {
    if (!drawing())
        return;

    cairo_save(cairo_context);

    // Draw the line with the desired color
    cairo_set_source_rgb(cairo_context, color.r/255.0f, color.g/255.0f, color.b/255.0f);
    cairo_move_to(cairo_context, line.a.x, line.a.y);
    cairo_line_to(cairo_context, line.b.x, line.b.y);

    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 2);
    cairo_stroke(cairo_context);

    cairo_restore(cairo_context);
}

void Renderer::draw_box(const Boxf &box) {
    if (!drawing())
        return;

    cairo_save(cairo_context);

    // Draw the rect
    cairo_set_source_rgb(cairo_context, 0.5, 0.0, 0.5);
    cairo_rectangle(cairo_context, box.min().x, box.min().y, box.width(), box.height());

    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 2);
    cairo_stroke(cairo_context);
    cairo_restore(cairo_context);
}

void Renderer::draw_poly(const Polyf &poly) {
    if (!drawing())
        return;

    // Start vertex
    cairo_save(cairo_context);
    cairo_move_to(cairo_context, poly.at(0).x, poly.at(0).y);

    // Draw the lines
    for (auto i = 1; i <= poly.size(); i++)
        cairo_line_to(cairo_context, poly.at(i).x, poly.at(i).y);

    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 2);
    cairo_stroke(cairo_context);
    cairo_restore(cairo_context);
}

void Renderer::draw_filled_poly(Polyf poly, const Color &color) {
    if (!drawing())
        return;

    // Start vertex
    cairo_save(cairo_context);
    cairo_move_to(cairo_context, poly.at(0).x, poly.at(0).y);

    // Draw the lines
    for (auto i = 1; i <= poly.size(); i++)
        cairo_line_to(cairo_context, poly.at(i).x, poly.at(i).y);

    // Draw the outline
    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 2);
    cairo_set_source_rgb(cairo_context, 0.24, 0.24, 0.24);
    cairo_stroke_preserve(cairo_context);

    // And fill
    cairo_set_source_rgb(cairo_context, color.r/255.0f, color.g/255.0f, color.b/255.0f);
    cairo_fill(cairo_context);
    cairo_restore(cairo_context);
}

void Renderer::draw_splitter(const Splitter &splitter) {
    if (!drawing())
        return;

    // Calculate the rise and the run of the splitter
    float dist = std::sqrt(splitter.dx*splitter.dx + splitter.dy*splitter.dy);
    float run  = splitter.dx / dist;
    float rise = splitter.dy / dist;

    dist = std::max(map_.size().x, map_.size().y);

    // Extend the splitter to reach both ends of the screen
    cairo_save(cairo_context);
    cairo_set_source_rgb(cairo_context, 0.5, 0.5, 0.0);

    cairo_move_to(cairo_context, splitter.p.x - run*dist, splitter.p.y - rise*dist);
    cairo_line_to(cairo_context, splitter.p.x + run*dist, splitter.p.y + rise*dist);

    cairo_identity_matrix(cairo_context);
    cairo_set_line_width(cairo_context, 4);

    cairo_stroke(cairo_context);
    cairo_restore(cairo_context);
}

void Renderer::draw_text(const std::string &text) {
    cairo_save(cairo_context);
    cairo_set_source_rgb(cairo_context, 1.0, 1.0, 1.0);
    cairo_identity_matrix(cairo_context);
    
    // Select the font
    cairo_select_font_face(cairo_context, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cairo_context, 16);
    
    // Add the map information beforehand
    std::stringstream ss;
    ss << "Map Name: " << map_.map() << "\n";
    ss << "Map Size: " << map_.size().x << "x" << map_.size().y << "\n";
    ss << "Vertices #: " << map_.num_vertices() << "\n";
    ss << "Linedefs #: " << map_.num_linedefs() << "\n";
    ss << "Sectors  #: " << map_.num_sectors() << "\n";
    ss << "\n" << text;

    std::string line;
    int y = 16;

    // Split at the newlines
    while (std::getline(ss, line, '\n')) {
        // Render the text
        cairo_move_to(cairo_context, 8, y);
        cairo_show_text(cairo_context, line.c_str());
        y += 16;
    }

    cairo_restore(cairo_context);
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

void Renderer::clear() {
    if (!drawing())
        return;

    cairo_set_source_rgb(cairo_context, 0.0, 0.0, 0.0);
    cairo_paint(cairo_context);
}

void Renderer::show() {
    if (!drawing())
        return;

    // Select the font
    cairo_save(cairo_context);
    cairo_set_source_rgb(cairo_context, 1.0, 1.0, 1.0);
    cairo_identity_matrix(cairo_context);
    cairo_select_font_face(cairo_context, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cairo_context, 10); 
    
    // Render the text
    cairo_move_to(cairo_context, 4, cairo_image_surface_get_height(cairo_surface)-1);
    cairo_show_text(cairo_context, "Generated by DOOM NodeBuilder - Copyright (C) 2022 Zach C.");
    cairo_restore(cairo_context);

    // Update the SDL texture with the contents of the Cairo surface
    auto data = cairo_image_surface_get_data(cairo_surface);
    SDL_UpdateTexture(texture, NULL, data, cairo_image_surface_get_stride(cairo_surface));

    // Clear and present the renderer
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
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
