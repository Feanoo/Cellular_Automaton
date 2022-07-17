#define N_TYPES 8


#define GAZ 128
#define LIQUID 64
#define SOLID 32

#define VOID 0
#define FIRE 1

#define SAND (0 | SOLID)
#define WOOD (1 | SOLID)

#define WATR (0 | LIQUID)

#define SMOK (0 | GAZ)
#define STEM (1 | GAZ)
#define CLOD (2 | GAZ)

extern unsigned char particle_types[N_TYPES];

void init_particles();