#include <SDL2/SDL.h>

#define BACKGROUND_TILE_SIZE 1024
#define VIDEO_WIDTH 256
#define VIDEO_HEIGHT 256

using namespace std;

enum RGBA { 
    BLACK = 0x000000FF, 
    GRAY = 0x545454FF, 
    LIGHT_GRAY = 0xA8A8A8FF,
    WHITE = 0xFFFFFFFF
};

class PPU{

public:
    PPU();
    ~PPU();

    void updateWindow(uint8_t const* const*, uint8_t const* const*);
    void draw(uint32_t*, uint8_t const* const*);

private:
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Surface *screenSurface;

};