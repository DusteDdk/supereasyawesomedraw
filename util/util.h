#include <SDL2/SDL.h>

typedef struct {
    int width;
    int height;
    int fullscreen;
    double aspect;
} settings_t;

void init(int argc, char **argv);


SDL_Renderer *getRenderer();
SDL_Window *getWindow();
settings_t *getSettings();
void setScreenSize(int w, int h);
void toggleFullscreen();

void brushInit();
void brushDown();
void brushUp();
void brushMove(int x, int y);
void brushDraw(SDL_Renderer *ren, SDL_Texture *tex, SDL_Texture *curTex);
void brushNextCol();
void brushToggleUseButton();