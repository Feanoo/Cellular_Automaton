#include "particles.h"

unsigned char particle_types[N_TYPES];

void init_particles(){
    particle_types[0] = VOID;
    particle_types[1] = SAND;
    particle_types[2] = WATR;
    particle_types[3] = WOOD;
    particle_types[4] = FIRE;
    particle_types[5] = SMOK;
    particle_types[6] = STEM;
    particle_types[7] = CLOD;
}