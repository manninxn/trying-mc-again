#ifndef WORLD_H
#define WORLD_H
#include <cglm/ivec3.h>
#include <windows.h>
#include "chunk.h"
#include "shader.h"


typedef struct {
	chunk** chunks;
	int light_level;
	int render_distance;
	int loaded_chunks;
	noise_params* noise;
	ivec3 loaded_position;
	HANDLE meshing_thread;
	HANDLE chunk_access_mutex;
	DWORD meshing_thread_id;
} world;



world* world_new(int render_distance);
void world_update_loaded_position(world* this, ivec3 new_pos);
void world_generate_chunk(world* this, ivec3 chunk_coordinate);
void world_update(world* this);
void world_render(world* this, shader* shader);
#define CHUNK_AT(world, x, y, z)  world->chunks[(x) * world->render_distance * world->render_distance + (y) * world->render_distance + (z)]

#endif