#include "draw.h"
#include <SDL2/SDL.h>

void draw_grid(SDL_Renderer* renderer, char* grid, int nx, int ny, int tile_size){
    SDL_Rect rect = {0, 0, tile_size, tile_size};
    for(int i=0; i<nx*ny; i++){
        rect.x = (i%nx) * tile_size;
        rect.y = (i/nx) * tile_size;
        if (grid[i] == 1){
            SDL_SetRenderDrawColor(renderer, 200, 100, 30, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        else if (grid[i] == 2){
            SDL_SetRenderDrawColor(renderer, 20, 100, 200, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        else if (grid[i] == 3){
            SDL_SetRenderDrawColor(renderer, 88, 53, 28, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        else if (grid[i] == 4){
            SDL_SetRenderDrawColor(renderer, 238, 53, 28, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        else if (grid[i] == 5){
            SDL_SetRenderDrawColor(renderer, 26, 4, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }

        // SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        // SDL_RenderDrawRect(renderer, &rect);
    }
}