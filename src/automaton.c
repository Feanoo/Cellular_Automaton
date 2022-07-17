#include "automaton.h"
#include "particles.h"
#include "draw.h"

void mainloop(SDL_Window* window, SDL_Renderer* renderer){
    int width, height;
    SDL_GetWindowSize(window, &width, &height);

    SDL_Event event;
    int running = 1;
    int tile_size = 10;
    int nx=(width / tile_size), ny=(height / tile_size), n_tiles=nx*ny;

    unsigned char* grid = malloc(sizeof(unsigned char) * n_tiles), *new_grid;
    memset(grid, 0, sizeof(char) * n_tiles);

    for (int j=1; j<=30; j++){
        for (int i=0; i<nx; i++){
            grid[10*nx + 10+j] = WOOD;
            grid[10*nx + 40+j] = WOOD;
            grid[nx*(ny - j) + i*(rand()%3 != 0)] = STEM;
        }
    }

    grid[11*nx + 11] = WOOD;
    grid[11*nx + 70] = WOOD;

    int pause=1;

    int add = 0;
    int add_type = SAND;
    int mousex, mousey;
    SDL_GetMouseState(&mousex, &mousey);

    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_p){
                    pause = !pause;
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                switch (event.button.button){
                    case 1:
                        add = 1;
                        break;
                    case 3:
                        add_type = (add_type+1)%N_TYPES;
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
            grid[tile] = particle_types[add_type];
        }

        if (!pause){
            new_grid = update(grid, nx, ny);
            free(grid);
            grid = new_grid;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        draw_grid(renderer, grid, nx, ny, tile_size);

        SDL_RenderPresent(renderer);
        SDL_Delay(50);
    }

    free(grid);
}

unsigned char* update(unsigned char* grid, int nx, int ny){
    unsigned char* new_grid = (unsigned char*)malloc(sizeof(unsigned char) * nx*ny);
    memset(new_grid, 0, nx*ny);
    for (int i=nx*ny-1; i>-1; i--){
        switch(grid[i]){
            case SAND:
                update_sand(grid, new_grid, i, nx, ny);
                break;
            case WATR:
                update_water(grid, new_grid, i, nx, ny);
                break;
            case WOOD:
                new_grid[i] = grid[i];
                break;
            case FIRE:
                update_fire(grid, new_grid, i, nx, ny);
                break;
            case SMOK:
                update_smoke(grid, new_grid, i, nx, ny);
                break;
            case STEM:
                update_steam(grid, new_grid, i, nx, ny);
                break;
            case CLOD:
                update_cloud(grid, new_grid, i, nx, ny);
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

void update_tile(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    long unsigned int n = get_tiles(grid, i, nx, ny);

    set_tiles(new_grid, i, get_new_tiles(n), nx, ny);
    // printf("%d\n", grid[1]);
}

void set_tiles(unsigned char* grid, int index, long unsigned int n, int nx, int ny){
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

long unsigned int get_tiles(unsigned char* grid, int index, int nx, int ny){
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

void update_sand(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;

    if (y < ny-1){
        if (!new_grid[i+nx]){// && !grid[i+nx]){
            new_grid[i+nx] = SAND;
            return;
        }

        /*sand should fall through liquids and gazes*/
        if (grid[i+nx] & LIQUID || grid[i+nx] & GAZ){
            new_grid[i] = grid[i+nx];
            new_grid[i+nx] = SAND;
            grid[i+nx] = VOID;
            return;
        }

        if (x > 0){
            if (!grid[i + nx - 1] && !new_grid[i + nx - 1]){
                new_grid[i + nx - 1] = SAND;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i + nx + 1] && !new_grid[i + nx + 1]){
                new_grid[i + nx + 1] = SAND;
                return;
            }
        }
    }

    new_grid[i] = grid[i];
}

void update_water(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;
    if (y < ny-1){
        if (!new_grid[i+nx]){
            new_grid[i+nx] = WATR;
            return;
        }

        /*water should fall through gazes*/
        if (grid[i+nx] & GAZ){
            new_grid[i] = grid[i+nx];
            new_grid[i+nx] = WATR;
            grid[i+nx] = VOID;
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

void update_fire(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;

    int n_neig = count_neig(grid, i, FIRE, nx, ny);

    int n_propag = 0;

    if (x > 0){
        if (grid[i - 1] == WOOD){
            grid[i - 1] = VOID;
            new_grid[i - 1] = FIRE;
            n_propag ++;
        }
        if (y > 0){
            if (grid[i-1-nx] == WOOD){
                grid[i-1-nx] = VOID;
                new_grid[i-1-nx] = FIRE;
                n_propag ++;
            }
            if (grid[i-nx] == WOOD){
                grid[i-nx] = VOID;
                new_grid[i-nx] = FIRE;
            }
            else if (grid[i - nx] == WATR){
                grid[i - nx] = VOID;
                new_grid[i] = STEM;
                return;
            }
        }
        if (y < ny-1){
            if (grid[i-1+nx] == WOOD){
                grid[i-1+nx] = VOID;
                new_grid[i-1+nx] = FIRE;
            }
            if (grid[i+nx] == WOOD){
                grid[i+nx] = VOID;
                new_grid[i+nx] = FIRE;
                n_propag ++;
            }
        }
    }

    if (x < nx-1){
        if (grid[i+1] == WOOD){
            grid[i+1] = VOID;
            new_grid[i+1] = FIRE;
            n_propag ++;
        }
        if (y > 0){
            if (grid[i+1-nx] == WOOD){
                grid[i+1-nx] = VOID;
                new_grid[i+1-nx] = FIRE;
                n_propag ++;
            }
        }
        if (y < ny-1){
            if (grid[i+1+nx] == WOOD){
                grid[i+1+nx] = VOID;
                new_grid[i+1+nx] = FIRE;
                n_propag ++;
            }
        }
    }

    if (n_propag > 0){
        new_grid[i] = SMOK;
        return;
    }
    if (n_neig > 2 && n_neig < 4){
        new_grid[i] = grid[i];
    }
}


void update_smoke(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;
    if (y > 0){
        if (!new_grid[i-nx] && !grid[i-nx]){
            new_grid[i-nx] = SMOK;
            return;
        }

        if (x > 0){
            if (!grid[i - nx - 1] && !new_grid[i - nx - 1]){
                new_grid[i - nx - 1] = SMOK;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i - nx + 1] && !new_grid[i - nx + 1]){
                new_grid[i - nx + 1] = SMOK;
                return;
            }

        }
    }

    if (rand()%2){
        if (x > 0){
            if (!grid[i - 1] && !new_grid[i-1]){
                new_grid[i - 1] = SMOK;
                return;
            }
        }
    }
    
    if (x < nx - 1){
        if (!grid[i + 1] && !new_grid[i + 1]){
            new_grid[i + 1] = SMOK;
            return;
        }
    }

    new_grid[i] = grid[i];
}

void update_steam(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;

    if (count_neig(grid, i, STEM, nx, ny) == 8){
        new_grid[i] = CLOD;
        return;
    }

    if (y > 0){
        if (!new_grid[i-nx] && !grid[i-nx]){
            new_grid[i-nx] = STEM;
            return;
        }

        if (x > 0){
            if (!grid[i - nx - 1] && !new_grid[i - nx - 1]){
                new_grid[i - nx - 1] = STEM;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i - nx + 1] && !new_grid[i - nx + 1]){
                new_grid[i - nx + 1] = STEM;
                return;
            }

        }
    }

    if (rand()%2){
        if (x > 0){
            if (!grid[i - 1] && !new_grid[i-1]){
                new_grid[i - 1] = STEM;
                return;
            }
        }
    }
    
    if (x < nx - 1){
        if (!grid[i + 1] && !new_grid[i + 1]){
            new_grid[i + 1] = STEM;
            return;
        }
    }

    new_grid[i] = grid[i];
}

void update_cloud(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny){
    int x = i%nx, y = i/nx;

    if (count_neig(grid, i, CLOD, nx, ny) == 8){
        new_grid[i] = WATR;
        return;
    }

    if (y > 0){
        if (!new_grid[i-nx] && !grid[i-nx]){
            new_grid[i-nx] = CLOD;
            return;
        }

        /*clouds are lighter than other gazes*/
        if (grid[i-nx] != CLOD && grid[i-nx] & GAZ){
            new_grid[i] = grid[i-nx];
            new_grid[i-nx] = CLOD;
            grid[i-nx] = VOID;
            return;
        }

        if (x > 0){
            if (!grid[i - nx - 1] && !new_grid[i - nx - 1]){
                new_grid[i - nx - 1] = CLOD;
                return;
            }

            /*clouds are lighter than other gazes*/
            if (grid[i - nx - 1] != CLOD && grid[i - nx - 1] & GAZ){
                new_grid[i] = grid[i - nx - 1];
                new_grid[i - nx - 1] = CLOD;
                grid[i - nx - 1] = VOID;
                return;
            }
        }


        if (x < nx - 1){
            if (!grid[i - nx + 1] && !new_grid[i - nx + 1]){
                new_grid[i - nx + 1] = CLOD;
                return;
            }

            /*clouds are lighter than other gazes*/
            if (grid[i - nx + 1] != CLOD && grid[i - nx + 1] & GAZ){
                new_grid[i] = grid[i - nx + 1];
                new_grid[i - nx + 1] = CLOD;
                grid[i - nx + 1] = VOID;
                return;
            }
        }
    }

    if (rand()%2){
        if (x > 0){
            if (!grid[i - 1] && !new_grid[i-1]){
                new_grid[i - 1] = CLOD;
                return;
            }
        }
    }
    
    if (x < nx - 1){
        if (!grid[i + 1] && !new_grid[i + 1]){
            new_grid[i + 1] = CLOD;
            return;
        }
    }

    new_grid[i] = grid[i];
}

int count_neig(unsigned char* grid, int i, unsigned char type, int nx, int ny){
    int x = i%nx, y = i/nx, n=0;
    if (x > 0){
        n += grid[i-1] == type;
        if (y > 0){
            n += grid[i-1-nx] == type;
            n += grid[i-nx] == type;
        }
        if (y < ny-1){
            n += grid[i-1+nx] == type;
            n += grid[i+nx] == type;
        }
    }

    if (x < nx-1){
        n += grid[i+1] == type;
        if (y > 0){
            n += grid[i+1-nx] == type;
        }
        if (y < ny-1){
            n += grid[i+1+nx] == type;
        }
    }

    // printf("%d\n", n);

    return n;
}