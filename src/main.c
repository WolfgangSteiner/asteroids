#include <stdio.h>
#include "SDL2/SDL.h"
#include "base_defines.h"
#include "game_state.h"
#include <stdlib.h>
#include "gfxa.h"
#include "transform2d.h"
#include "vec2i.h"

void on_draw(game_state_t* t);

void render_frame_buffer(SDL_Surface* surface, frame_buffer_t* fb) {
    u32* dst = (u32*)surface->pixels;
    u32 pitch = surface->pitch / sizeof(u32);

    if (fb->type == FRAME_BUFFER_INDEXED) {
        for (u32 y = 0; y < fb->height; ++y) {
            for (u32 x = 0; x < fb->width; ++x) {
                u8 index = fb->data[y * fb->width + x];
                color_rgba_t color = color_palette_map(&fb->palette, index);
                dst[y * pitch + x] = color.rgba;
            }
        }
    } else if (fb->type == FRAME_BUFFER_RGBA) {
        u32* src = (u32*)fb->data;
        for (u32 y = 0; y < fb->height; y++) {
            for (u32 x = 0; x < fb->width; ++x) {
                dst[y * pitch + x] = src[y * fb->width + x];
            }
        }
    }
}


u8 rand_u8() {
    return (u8)(rand() % 256);
}


#define WINDOW_WIDTH 1024   
#define WINDOW_HEIGHT 1024

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    game_state_t game_state;
    game_state_init(&game_state);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("ASTEROIDS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);    
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface* surface = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0, 0, 0, 0); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);


    while(game_state.is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                game_state.is_running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        on_draw(&game_state);
        
        render_frame_buffer(surface, &game_state.frame_buffer);
        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        // Render the texture into the window:
        SDL_Rect srcrect = { 0, 0, game_state.frame_buffer.width, game_state.frame_buffer.height };
        SDL_Rect dstrect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }

    SDL_Quit();
    return 0;
}