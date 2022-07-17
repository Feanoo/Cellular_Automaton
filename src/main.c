#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "automaton.h"
#include "particles.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;
const int SCREEN_BPP = 32;

SDL_Window* screen = NULL;
SDL_Renderer* renderer = NULL;

int init(){
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1){
        return -1;
    }

    if (TTF_Init() == -1){
        return -1;
    }

    screen = SDL_CreateWindow("Cellular Automaton",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (screen == NULL){
        return -1;
    }

    renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        return -1;
    }

    init_particles();

    return 0;

}

void Quit(){
    SDL_Quit();
}

int main(){
    printf("initializing SDL...");
    if (init()){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, SDL_GetError());
        return -1;
    }
    printf("done\n");

    printf("start\n");
    mainloop(screen, renderer);

    Quit();
    return 0;
}