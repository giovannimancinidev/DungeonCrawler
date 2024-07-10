#include <SDL.h>

// Struct declaration
typedef struct actor{
    float x;
    float y;
    int velocity;
    int tile_x;
    int tile_y;
}actor;

// Fucntion declaration
int get_tile(const int offsetX, const int offsetY, const int width, const int height, int* xCoord, int *yCoord);

int draw_tile(SDL_Renderer *renderer, SDL_Texture *texture, const int tile_x, const int tile_y, const float x, const float y);
int draw_actor(SDL_Renderer *renderer, SDL_Texture *texture, actor *actor);
int load_font(SDL_Renderer *renderer, const char *path);
int draw_text(SDL_Renderer *renderer, const float x, const float y, const char *str);
int draw_room(SDL_Renderer* renderer, SDL_Texture* texture, const float room_x, const float room_y, const unsigned char bitmask);