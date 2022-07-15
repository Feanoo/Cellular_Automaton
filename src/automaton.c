#include "automaton.h"
#include "draw.h"

#define VOID 0
#define SAND 1
#define WATR 2

void mainloop(SDL_Window* window, SDL_Renderer* renderer){
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_Event event;
    int running = 1;
    int tile_size = 10;
    int nx=(width / tile_size), ny=(height / tile_size), n_tiles=nx*ny;

    char* grid = malloc(sizeof(char) * n_tiles), *new_grid;
    memset(grid, 0, sizeof(char) * n_tiles);

    for (int i=50; i>=0; i--){
        grid[10 + i] = SAND;
        grid[nx + 10 + i] = SAND;
        grid[nx*(i+5) + 10] = SAND;
        grid[nx*(i+5) + 20] = WATR;
        grid[nx*(i+5) + 30] = WATR;
        grid[nx*(i+5) + 40] = WATR;
        grid[nx*(i+5) + 50] = WATR;
        grid[nx*(i+5) + 60] = WATR;
    }


    int add = 0;
    int add_type = SAND;
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);

    draw_grid(renderer, grid, nx, ny, tile_size);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);

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
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                switch (event.button.button){
                    case 1:
                        add = 1;
                        break;
                    case 3:
                        if (add_type == SAND){
                            add_type = WATR;
                        }
                        else{
                            add_type = SAND;
                        }
                        break;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP){
                add = 0;
            }
            else if (event.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mousex, &mousey);
            }
        }

        if (add){
            int tile = get_tile(mousex, mousey, nx, ny, tile_size);
            if (!grid[tile]){
                grid[tile] = add_type;
            }
        }

        new_grid = update(grid, nx, ny);
        free(grid);
        grid = new_grid;

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer, grid, nx, ny, tile_size);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    free(grid);
}

char* update(char* grid, int nx, int ny){
    char* new_grid = (char*)malloc(sizeof(char) * nx*ny);
    memset(new_grid, 0, nx*ny);
    for (int i=nx*ny-1; i>-1; i--){
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

int get_tile(int x, int y, int nx, int ny, int tile_size){
    x /= tile_size;
    y /= tile_size;
    return y*nx + x;
}

void update_tile(char* grid, char* new_grid, int i, int nx, int ny){
    long unsigned int n = get_tiles(grid, i, nx, ny);

    set_tiles(new_grid, i, get_new_tiles(n), nx, ny);
    // printf("%d\n", grid[1]);
}

void set_tiles(char* grid, int index, long unsigned int n, int nx, int ny){
    int tile;
     for (int i=-1; i<2; i++){
        for (int j=-1; j<2; j++){
            tile = n & 127;
            n = n >> 7;

            int ind = index + j + nx*i;
            if(ind >= nx*ny || ind < 0){
                continue;
            }
            //127 c'est le bord
            if (tile > 127){
                printf("error : tile=%d\n", tile);
                continue;
            }
            if (tile != 127 && !grid[ind]){
                if (tile){
                    printf("%d : %d\n", ind, tile);
                }
                grid[ind] = (char)tile;
            }
        }
    }
}

long unsigned int get_new_tiles(long unsigned int n){
    return n;
}

long unsigned int get_tiles(char* grid, int index, int nx, int ny){
    long unsigned int tile = 127;

    int begin_i=-1, begin_j=-1;
    int end_i=2, end_j=2;

    //on represente le bord 1111111 = 127

    //le haut est au bord
    if (index < nx){
        tile = (tile | 127) << 7;
        tile = (tile | 127) << 7;
        tile = (tile | 127) << 7;
        begin_i = 0;
    }

    //le cote gauche est au bord
    if (index % nx == 0){
        begin_j = 0;
    }

    //le cote droit est au bord
    if (index % nx == nx-1){
        end_j = 1;
    }

    //le bas est au bord
    if (index / nx == ny - 1){
        end_i = 1;
    }

    for (int i=begin_i; i<end_i; i++){
        for (int j=begin_j; j<end_j; j++){
            if ((!begin_j && !j) || (end_j == 1 && j==1) || (end_i == 1 && i==1)){
                tile = (tile | 127) << 7;
            }
            tile = tile | grid[index + j + nx*i];
            tile = tile << 7;
            if (grid[index + j + nx*i]){
                printf("i : %d, j : %d,  n = %d\n", i, j, grid[index + j + nx*i]);
            }
        }
    }

    return tile;
}

void update_sand(char* grid, char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;

    if (y < ny-1){
        if (!new_grid[i+nx]){// && !grid[i+nx]){
            new_grid[i+nx] = SAND;
            return;
        }

        /*sand should fall through water*/
        if (grid[i+nx] == WATR){
            new_grid[i] = WATR;
            new_grid[i+nx] = SAND;
            grid[i+nx] = VOID;
            return;
        }

        if (x > 0){
            if (!grid[i + nx - 1] && !new_grid[i + nx - 1]){
                new_grid[i + nx - 1] = SAND;
                return;
            }

            /*fall through water*/
            if (grid[i + nx - 1] == 2){
                new_grid[i + nx - 1] = SAND;
                grid[i + nx - 1] = VOID;
                new_grid[i] = WATR;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i + nx + 1] && !new_grid[i + nx + 1]){
                new_grid[i + nx + 1] = SAND;
                return;
            }

            /*fall through water*/
            if (grid[i + nx + 1] == 2){
                new_grid[i + nx + 1] = SAND;
                grid[i + nx + 1] = VOID;
                new_grid[i] = WATR;
                return;
            }
        }
    }

    new_grid[i] = grid[i];
}

void update_water(char* grid, char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;
    if (y < ny-1){
        if (!new_grid[i+nx]){
            new_grid[i+nx] = WATR;
            return;
        }

        if (x > 0){
            if (!grid[i + nx - 1] && !new_grid[i + nx - 1]){
                new_grid[i + nx - 1] = WATR;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i + nx + 1] && !new_grid[i + nx + 1]){
                new_grid[i + nx + 1] = WATR;
                return;
            }

        }
    }

    if (rand()%2){
        if (x > 0){
            if (!grid[i - 1] && !new_grid[i-1]){
                new_grid[i - 1] = WATR;
                return;
            }
        }
    }
    
    if (x < nx - 1){
        if (!grid[i + 1] && !new_grid[i + 1]){
            new_grid[i + 1] = WATR;
            return;
        }
    }

    new_grid[i] = grid[i];
}
