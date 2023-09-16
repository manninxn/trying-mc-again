#ifndef WORLD_H
#define WORLD_H
#include <cglm/ivec3.h>

struct chunk;

typedef struct {
	chunk** loaded_chunks;
	int render_distance;
	vec3 loaded_position;
} world;


world* world_new(int render_distance);
void world_update_loaded_position(world* this, vec3 new_pos);

#define GET_CHUNK(world, x, y, z) 

#endif