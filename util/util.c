#include "util.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

int min(int a, int b) {
    return a>b?b:a;
}
int max(int a, int b) {
    return a>b?a:b;
}

SDL_Renderer *mainRenderer;
SDL_Window *mainWindow;

settings_t settings;

void init(int argc, char **argv) {
    printf("Program initialization\n\t%i arguments:\n", argc);
    for(int i=0; i < argc; i++) {
        printf("\t\t[%i] %s\n", i, argv[i]);
    }

    setScreenSize(settings.width, settings.height);

    printf("Settings:\n\tFullscreen: %i\n\tWidth: %i\n\tHeight: %i\n\tAspect: %f\n",
        settings.fullscreen,
        settings.width,
        settings.height,
        settings.aspect);

    SDL_Init(SDL_INIT_VIDEO);


    int windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    switch(settings.fullscreen) {
        case 1:
            windowFlags |= SDL_WINDOW_FULLSCREEN;
            break;
        case 2:
            windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
            break;
    }

    mainWindow = SDL_CreateWindow("win",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        settings.width,
        settings.height,
        windowFlags
    );
    SDL_SetWindowMinimumSize(mainWindow, 320, 240);

    SDL_ShowCursor(0);

    if(mainWindow == NULL) {
        printf("Window create err: %s\n", SDL_GetError());
    }

    mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    if(mainRenderer == NULL) {
        printf("Renderer create err: %s\n", SDL_GetError());
    }

}

void toggleFullscreen() {
    settings.fullscreen = !settings.fullscreen;
    int flag = settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0;
    SDL_SetWindowFullscreen(mainWindow, flag);
}

SDL_Renderer *getRenderer() {
    return mainRenderer;
}

SDL_Window *getWindow() {
    return mainWindow;
}

settings_t *getSettings() {
    return &settings;
}

void setScreenSize(int w, int h) {
    settings.width=w;
    settings.height=h;
    settings.aspect = (double)w/(double)h;
}

typedef struct {
    uint_fast8_t red;
    uint_fast8_t green;
    uint_fast8_t blue;
} col_t;

int numCols=7;
col_t cols[] = {
    {0,0,255},
    {0,255,0},
    {0,255,255},
    {255,0,0},
    {255,0,255},
    {255,255,0},
    {255,255,255}
};

struct {
    uint_fast8_t isDown;
    int x;
    int y;
    int lx;
    int ly;
    int_fast8_t colIdx;
    uint_fast8_t useButton;
} brSt;

void brushNextCol() {
    brSt.colIdx++;
    if(brSt.colIdx==numCols) {
        brSt.colIdx=0;
    }
}
void brushPrevCol() {
    brSt.colIdx--;
    if(brSt.colIdx < 0) {
        brSt.colIdx=numCols-1;
    }
}

void brushInit() {
    memset(&brSt, 0, sizeof(brSt));
    brSt.colIdx = 0;
    brSt.useButton=1;
}

void brushDown() {
    brSt.isDown=1;

}

void brushUp() {
    brSt.isDown=0;
}

void brushMove(int x, int y) {
    brSt.x = x;
    brSt.y = y;
}

void brushToggleUseButton() {
    brSt.useButton = !brSt.useButton;
}

void brushDraw(SDL_Renderer *ren, SDL_Texture *tex, SDL_Texture *curTex) {


    SDL_SetTextureColorMod(curTex, cols[brSt.colIdx].red,cols[brSt.colIdx].green,cols[brSt.colIdx].blue);
    if(brSt.useButton || brSt.isDown)
    {
        SDL_Rect test = {
            .x = brSt.x,
            .y = brSt.y,
            .w = 16,
            .h = 16
        };

        SDL_SetRenderTarget(ren, tex);
        SDL_SetRenderDrawColor(ren, cols[brSt.colIdx].red,cols[brSt.colIdx].green,cols[brSt.colIdx].blue,255);


          if(brSt.lx == brSt.x && brSt.ly == brSt.y) {
                SDL_RenderFillRect(ren, &test );
            } else {
                float x1 = brSt.lx;
                float x2 = brSt.x;
                float y1 = brSt.ly;
                float y2 = brSt.y;

                float a = x2-x1;
                float b = y2-y1;

                float len = sqrt (a*a+b*b );

                float nx = (brSt.x-brSt.lx)/len;
                float ny = (brSt.y-brSt.ly)/len;
   
                for(int s = 0; s < len; s++) {
                    test.x = (x1 + s*nx);
                    test.y = (y1 + s*ny);
                    SDL_RenderFillRect(ren, &test );
                }

         }
        SDL_SetRenderTarget(ren, NULL);
    }

    brSt.lx = brSt.x;
    brSt.ly = brSt.y;

}


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif

void save(SDL_Texture* tex) {
    char fileName[17];

    FILE *fd;

    for(int i=0; i < 1000; i++) {
        sprintf(fileName, "drawing_%03d.bmp", i);
        fd = fopen(fileName, "r");
        if( !fd ) {
            SDL_SetRenderTarget(mainRenderer, tex);
            SDL_Surface* surf = SDL_CreateRGBSurface(0, settings.width, settings.height, 32, rmask, gmask, bmask, amask);
            SDL_RenderReadPixels(mainRenderer, NULL,SDL_PIXELFORMAT_ABGR8888, surf->pixels, surf->pitch );
            SDL_SaveBMP(surf, fileName);
            SDL_SetRenderTarget(mainRenderer, NULL);
            printf("Saved %s\n", fileName);
            return;
        }
    }
    printf("You saved too many drawings! Clean up and try again.\n");
}