#include <SDL2/SDL.h>

void mainloop(SDL_Window* screen, SDL_Renderer* renderer);
char* update(char *grid, int nx, int ny);
void update_sand(char* grid, char* new_grid, int i, int nx, int ny);
void update_water(char* grid, char* new_grid, int i, int nx, int ny);
void update_fire(char* grid, char* new_grid, int i, int nx, int ny);
void update_smoke(char* grid, char* new_grid, int i, int nx, int ny);

int get_tile(int x, int y, int nx, int ny, int tile_size);
int count_neig(char* grid, int i, char type, int nx, int ny);

void update_tile(char* grid, char* new_grid, int i, int nx, int ny);
void set_tiles(char* grid, int index, long unsigned int n, int nx, int ny);
long unsigned int get_new_tiles(long unsigned int n);
long unsigned int get_tiles(char* grid, int index, int nx, int ny);