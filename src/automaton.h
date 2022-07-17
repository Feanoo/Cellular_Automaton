#include <SDL2/SDL.h>

void mainloop(SDL_Window* screen, SDL_Renderer* renderer);
unsigned char* update(unsigned char *grid, int nx, int ny);
void update_sand(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void update_water(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void update_fire(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void update_smoke(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void update_steam(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void update_cloud(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);

int get_tile(int x, int y, int nx, int ny, int tile_size);
int count_neig(unsigned char* grid, int i, unsigned char type, int nx, int ny);

void update_tile(unsigned char* grid, unsigned char* new_grid, int i, int nx, int ny);
void set_tiles(unsigned char* grid, int index, long unsigned int n, int nx, int ny);
long unsigned int get_new_tiles(long unsigned int n);
long unsigned int get_tiles(unsigned char* grid, int index, int nx, int ny);