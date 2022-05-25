#include "automaton.h"
#include "draw.h"

void mainloop(SDL_Window* window, SDL_Renderer* renderer){
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    // printf("%d, %d\n", width, height);

    SDL_Event event;
    int running = 1;
    int tile_size = 15;
    int nx=(width / tile_size), ny=(height / tile_size), n_tiles=nx*ny;

    char* grid = malloc(sizeof(char) * n_tiles), *temp;
    memset(grid, 0, sizeof(char) * n_tiles);

    // grid[0] = 1;
    // grid[nx*5] = 1;

    for (int i=50; i>=0; i--){
        grid[nx*i + 20] = 2;
        grid[nx*i + 2] = 1;
        grid[nx*i + 30] = 1;
    }

    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    running = 0;
                }
            }
        }

        temp = update(grid, nx, ny);
        free(grid);
        grid = temp;

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer, grid, nx, ny, tile_size);

        SDL_RenderPresent(renderer);
        SDL_Delay(60);
    }

    free(grid);
}

char* update(char* grid, int nx, int ny){
    char* new_grid = (char*)malloc(sizeof(char) * nx*ny);
    memset(new_grid, 0, nx*ny);
    for (int i=nx*ny-1; i>-1; i--){
        // new_grid[i] = grid[i];
        switch(grid[i]){
            case 1:
                update_sand(grid, new_grid, i, nx, ny);
                break;
            case 2:
                update_water(grid, new_grid, i, nx, ny);
                break;
        }
    }

    return new_grid;
}

void update_sand(char* grid, char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;
    // printf("%d, %d, %d\n", x, y, ny);
    if (y < ny-1){
        if (!new_grid[i + nx] || !grid[i + nx]){
            // new_grid[i] = 0;
            new_grid[i + nx] = 1;
            return;
        }
        if (x > 0){
            if (!new_grid[i + nx - 1] || !grid[i + nx - 1]){
                // new_grid[i] = 0;
                new_grid[i + nx - 1] = 1;
                return;
            }
        }
        if (x < nx){
            if (!new_grid[i + nx + 1] || !grid[i + nx + 1]){
                // new_grid[i] = 0;
                new_grid[i + nx + 1] = 1;
                return;
            }
        }
    }

    new_grid[i] = grid[i];
}

void update_water(char* grid, char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;
    // printf("%d\n", i);
    if (y < ny-1){
        if (!new_grid[i + nx] || !grid[i + nx]){
            // new_grid[i] = 0;
            new_grid[i + nx] = 2;
            return;
        }
        if (x > 0){
            if (!new_grid[i + nx - 1] || !grid[i + nx - 1]){
                // new_grid[i] = 0;
                new_grid[i + nx - 1] = 2;
                return;
            }
        }
        if (x < nx){
            if (!new_grid[i + nx + 1] || !grid[i + nx + 1]){
                // new_grid[i] = 0;
                new_grid[i + nx + 1] = 2;
                return;
            }
        }
    }

    if (x > 0){
        if (!new_grid[i - 1] && !grid[i-1]){
            // printf("pute, %d, %d, %d | ", i, x, y);
            // printf("new grid i, i-1 : %d, %d | ", new_grid[i], new_grid[i-1]);
            // printf("grid i, i-1 : %d, %d\n", grid[i], grid[i-1]);
            new_grid[i - 1] = 2;
            return;
        }
    }

    if (x < nx){
        if (!new_grid[i + 1]){
            new_grid[i + 1] = 2;
            return;
        }
    }

    new_grid[i] = grid[i];
}
