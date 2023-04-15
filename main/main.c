#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


#include "../util/util.h"

int main(int argc, char** argv) {

    getSettings()->fullscreen=0;
    getSettings()->width=800;
    getSettings()->height=600;
    init(argc, argv);


    SDL_Renderer *ren = getRenderer();

    
    uint_fast8_t run = 1;

    SDL_GL_SetSwapInterval(1);

    SDL_Texture* curTex = IMG_LoadTexture(ren, "cur.png");
    SDL_Rect curRect = {
        .x = 0,
        .y = 0
    };
    SDL_QueryTexture(curTex, NULL, NULL, &(curRect.w), &(curRect.h));

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");


    brushInit(curTex, ren);

    SDL_Texture *texTarget = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, getSettings()->width, getSettings()->height);

    printf("\nSuper Easy Awesome Draw!\n"
        "---\n"
        "Clear screen : Backspace\n"
        "Change color : Spacebar or mouse-wheel\n"
        "Quit program: Ctrl + q\n"
        "Toggle using mouse button to draw : Ctrl + Enter\n"
        "Fullscreen : Ctrl + f\n"
        "Save picture ( ./drawing_N.bmp ) : Ctrl + s\n");

    SDL_SetRenderDrawColor(ren,0,0,0,255);
    SDL_SetRenderTarget(ren, texTarget);
    SDL_RenderFillRect(ren, NULL);
    SDL_SetRenderTarget(ren, NULL);

    while(run) {

        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_SPACE) {
                        brushNextCol();
                    }
                    if(event.key.keysym.sym == SDLK_BACKSPACE ) {
                        SDL_SetRenderDrawColor(ren,0,0,0,255);
                        SDL_SetRenderTarget(ren, texTarget);
                        SDL_RenderFillRect(ren, NULL);
                        SDL_SetRenderTarget(ren, NULL);
                    }
                    if(SDL_GetModState() & KMOD_LCTRL) {
                        switch(event.key.keysym.sym) {
                            case SDLK_q:
                                printf("Bye!\n");
                                return 0;
                                break;
                            case SDLK_RETURN:
                                brushToggleUseButton();
                                break;
                            case SDLK_f:
                                toggleFullscreen();
                                break;
                            case SDLK_s:
                                save(texTarget);
                                break;
                        }
                    }
                break;
                case SDL_WINDOWEVENT:
                    if( event.window.event == SDL_WINDOWEVENT_RESIZED ) {
                        setScreenSize(event.window.data1, event.window.data2);
                        SDL_Texture *newTex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, getSettings()->width, getSettings()->height);
                        SDL_SetRenderTarget(ren, newTex);
                        SDL_RenderCopy(ren, texTarget, NULL, NULL);
                        SDL_SetRenderTarget(ren, NULL);
                        SDL_DestroyTexture(texTarget);
                        texTarget=newTex;
                    }
                break;
                case SDL_MOUSEBUTTONDOWN:
                    brushDown();
                break;
                case SDL_MOUSEBUTTONUP:
                    brushUp();
                break;
                case SDL_MOUSEMOTION:
                    brushMove( event.motion.x, event.motion.y);
                break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) {
                        brushPrevCol();
                    }
                    if(event.wheel.y < 0) {
                        brushNextCol();
                    }
                break;

            }
        }

        uint32_t mbtn = SDL_GetMouseState( &(curRect.x), &(curRect.y));
        

        brushDraw(ren, texTarget, curTex);

        SDL_RenderCopy(ren, texTarget, NULL, NULL);
        SDL_RenderCopy(ren, curTex, NULL, &curRect);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(getWindow());
    SDL_Quit();

    return 0;
}
