#include <SDL.h>
#include "dungeon.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "maze.h"

extern float scale;
extern float camera_x;
extern float camera_y;
extern int draw_tile_counter;

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Error initializing SDL2: %s", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("DungeonCrawler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);

    if (!window)
    {
        SDL_Log("Error creating SDL2 Window: %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        SDL_Log("Error creating SDL2 Renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Log("Dungeon Game version 0.0.1!");

    int window_opened = 1;

    int width,height,channels;

    unsigned char *pixels = stbi_load("2D Pixel Dungeon Asset Pack/character and tileset/Dungeon_Tileset.png", &width, &height, &channels, 0);

    if (pixels == NULL)
    {
        SDL_Log("Error loading image");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Log("Image size: W %d, H %d C: %d\n", width, height, channels);
    
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

    if (texture == NULL)
    {
        SDL_Log("Error creating SDL2 Texture: %s", SDL_GetError());
        stbi_image_free(pixels);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_SetTextureBlendMode(texture,SDL_BLENDMODE_BLEND);
    
    if (SDL_UpdateTexture(texture, NULL, pixels, width * 4) != 0)
    {
        SDL_Log("Error update texture: %s", SDL_GetError());
        stbi_image_free(pixels);
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    stbi_image_free(pixels);

    actor first;
    first.x = 0;
    first.y = 0;
    first.tile_x = 9;
    first.tile_y = 9;
    first.velocity = 100;

    actor second;
    second.x = 0;
    second.y = 0;
    second.tile_x = 8;
    second.tile_y = 9;
    second.velocity = 200;

    load_font(renderer, "IMMORTAL.ttf");
    
    const Uint64 freq = SDL_GetPerformanceFrequency();
    double deltaTime = 0;

    Uint64 start = SDL_GetPerformanceCounter();

    while (window_opened)
    {
        draw_tile_counter = 0;

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                window_opened = 0;
            }
            if(event.type == SDL_MOUSEWHEEL)
            {
                scale += event.wheel.y * 0.1f;
                if(scale < 0.1f)
                {
                    scale = 0.1f;
                }
            }
        }

        const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

        if(keyboard[SDL_SCANCODE_UP])
        {
            first.y-=(first.velocity * deltaTime);
        }

        if(keyboard[SDL_SCANCODE_DOWN])
        {
            first.y+=(first.velocity * deltaTime);
        }

        if(keyboard[SDL_SCANCODE_LEFT])
        {
            first.x-=(first.velocity * deltaTime);
        }

        if(keyboard[SDL_SCANCODE_RIGHT])
        {
            first.x+=(first.velocity * deltaTime);
        }

        if(keyboard[SDL_SCANCODE_W])
        {
            second.y-=(second.velocity * deltaTime);
            camera_y--;
        }

        if(keyboard[SDL_SCANCODE_S])
        {
            second.y+=(second.velocity * deltaTime);
            camera_y++;
        }

        if(keyboard[SDL_SCANCODE_A])
        {
            second.x-=(second.velocity * deltaTime);
            camera_x--;
        }

        if(keyboard[SDL_SCANCODE_D])
        {
            second.x+=(second.velocity * deltaTime);
            camera_x++;
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderClear(renderer);
        
        for(int y = 0; y < 20; y++)
        {
            for(int x = 0; x < 20; x++)
            {
                draw_room(renderer, texture, 16 * 8 * scale * x, 16 * 8 * scale * y, maze[y][x]);
            }
        }

        draw_actor(renderer, texture, &first);
        draw_actor(renderer, texture, &second);

        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetPerformanceCounter();

        deltaTime = ((double)end - (double)start) / (double)freq;

        start = end;

        //SDL_Log("Draw tile amount: %d\n", draw_tile_counter);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}