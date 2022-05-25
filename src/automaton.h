#include <SDL2/SDL.h>

void mainloop(SDL_Window* screen, SDL_Renderer* renderer);
char* update(char *grid, int nx, int ny);
void update_sand(char* grid, char* new_grid, int i, int nx, int ny);
void update_water(char* grid, char* new_grid, int i, int nx, int ny);