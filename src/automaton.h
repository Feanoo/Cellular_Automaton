#include <SDL2/SDL.h>

void mainloop(SDL_Window* screen, SDL_Renderer* renderer);
char* update(char *grid, int nx, int ny);
void update_sand(char* grid, char* new_grid, int i, int nx, int ny);
void update_water(char* grid, char* new_grid, int i, int nx, int ny);
void update_tile(char* grid, char* new_grid, int i, int nx, int ny);
void set_tiles(char* grid, int index, long unsigned int n, int nx, int ny);
long unsigned int get_new_tiles(long unsigned int n);
long unsigned int get_tiles(char* grid, int index, int nx, int ny);