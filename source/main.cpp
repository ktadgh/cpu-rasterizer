#include <SDL2/SDL.h>
#include <algorithm>
#include <rasterizer/renderer.hpp>
#include <rasterizer/types.hpp>

#include <chrono>
#include <iostream>
int main()
{
    using clock = std::chrono::high_resolution_clock; // is there a reason this is in main?

    auto last_frame_start = clock::now();

    SDL_Surface * draw_surface = nullptr;
    SDL_Init(SDL_INIT_VIDEO);

    int width = 800;
    int height = 600;

    SDL_Window * window = SDL_CreateWindow("Tiny rasterizer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);

    int mouse_x = 0;
    int mouse_y = 0;

    bool running = true;
    while (running)
    {
        auto now = clock::now();
        float dt = std::chrono::duration_cast<std::chrono::duration<float>>(
            now - last_frame_start).count();
        last_frame_start = now;

        std::cout << dt << std::endl;

        for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type)
        {
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                if (draw_surface){
                    SDL_FreeSurface(draw_surface);
                }
                width = event.window.data1;
                height = event.window.data2;
                break;
            }
            break;
        case SDL_QUIT:
            running = false;
            break;
        case SDL_MOUSEMOTION:
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
            break;
        }

        if (!running)
            break;
        
        if (!draw_surface){
            draw_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height,
                32, SDL_PIXELFORMAT_RGBA32);
            SDL_SetSurfaceBlendMode(draw_surface, SDL_BLENDMODE_NONE);
        
        }
    SDL_Rect rect{.x = 0, .y = 0, .w = width, .h = height};
    SDL_BlitSurface(draw_surface, &rect, SDL_GetWindowSurface(window), &rect);
    SDL_UpdateWindowSurface(window);
    std::fill_n((uint32_t *)draw_surface->pixels, width * height, 0xffffdfdf); // dereferences and calls the function

    rasterizer::image_view color_buffer
    {
        .pixels = (rasterizer::color4ub *)draw_surface->pixels,
        .width = (std::uint32_t)width,
        .height = (std::uint32_t)height,
    };
    using namespace rasterizer;


    clear(color_buffer, {0.8f, 0.9f, 1.f, 1.f});

    viewport viewport
    {
        .xmin = 0,
        .ymin = 0,
        .xmax = (std::int32_t)color_buffer.width,
        .ymax = (std::int32_t)color_buffer.height,
    };
    
    clear(color_buffer, {0.9f, 0.9f, 0.9f, 1.f});
    
    vector3f positions[]
    {
        { 0.f,   0.5f, 0.f},
        {-0.5f, -0.5f, 0.f},
        { 0.5f, -0.5f, 0.f},
    };
    
    vector4f colors[]
    {
        {1.f, 0.f, 0.f},
        {0.f, 1.f, 0.f},
        {0.f, 0.f, 1.f},
    };
    
    draw(color_buffer, viewport,
        draw_command {
            .mesh = {
                .positions = {positions},
                .colors = {colors},
                .vertex_count = 3,
            },
        }
    );
    SDL_BlitSurface(draw_surface, &rect,
        SDL_GetWindowSurface(window), &rect);

    SDL_UpdateWindowSurface(window);
    }
}