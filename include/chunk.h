#ifndef CHUNK_H
#define CHUNK_H
#include "vbo.h"
#include "vao.h"
#include <cglm/vec3.h>
#define CHUNK_SIZE 32
#define BLOCKS_PER_CHUNK CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE
typedef struct {
	unsigned char blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	vec3 pos;
	vbo* vbo;
	vbo* ebo;
	vao* vao;
	unsigned int num_elems;
} chunk;

#define chunk_block_at(chunk, x, y, z) (chunk->blocks[((x) * CHUNK_SIZE * CHUNK_SIZE) + ((y) * CHUNK_SIZE) + (z)])
#define block_in_bounds(x,y,z) (x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE)
static const ivec3 faces[6] = {
	{0,0,1},
	{0,0,-1},
	{1, 0, 0},
	{-1, 0, 0},
	{0, 1, 0},
	{0, -1, 0},
};



static const unsigned int FACE_INDICES[] =
{ 1, 0, 3, 1, 3, 2 };

static const unsigned int UNIQUE_INDICES[] = { 1, 0, 5, 2 };
static const unsigned int CUBE_INDICES[] = {
	4, 5, 6, 4, 6, 7, // north (+z)
	1, 0, 3, 1, 3, 2, // south (-z)
	5, 1, 2, 5, 2, 6, // east (+x)
	0, 4, 7, 0, 7, 3, // west (-x)
	2, 3, 7, 2, 7, 6, // top (+y)
	5, 4, 0, 5, 0, 1, // bottom (-y)
};

static const unsigned int CUBE_INDICES_FLIPPED[] = {
	4, 5, 7, 7, 5, 4,
	1, 0, 2, 2, 0, 1,
	5, 1, 6, 6, 1, 5,
	0, 4, 3, 3, 4, 0,
	2, 3, 6, 6, 3, 2,
	5, 4, 1, 1, 4, 5,
};

static const int AO_NEIGHBORS[] = {
1, 0, 1,
0, -1, 1,
1, -1, 1,
-1, 0, 1,
0, -1, 1,
-1, -1, 1,
-1, 0, 1,
0, 1, 1,
-1, 1, 1,
1, 0, 1,
0, 1, 1,
1, 1, 1,
-1, 0, -1,
0, -1, -1,
-1, -1, -1,
1, 0, -1,
0, -1, -1,
1, -1, -1,
1, 0, -1,
0, 1, -1,
1, 1, -1,
-1, 0, -1,
0, 1, -1,
-1, 1, -1,
1, -1, 0,
1, 0, -1,
1, -1, -1,
1, -1, 0,
1, 0, 1,
1, -1, 1,
1, 1, 0,
1, 0, 1,
1, 1, 1,
1, 1, 0,
1, 0, -1,
1, 1, -1,
-1, -1, 0,
-1, 0, 1,
-1, -1, 1,
-1, -1, 0,
-1, 0, -1,
-1, -1, -1,
-1, 1, 0,
-1, 0, -1,
-1, 1, -1,
-1, 1, 0,
-1, 0, 1,
-1, 1, 1,
0, 1, -1,
-1, 1, 0,
-1, 1, -1,
0, 1, -1,
1, 1, 0,
1, 1, -1,
0, 1, 1,
1, 1, 0,
1, 1, 1,
0, 1, 1,
-1, 1, 0,
-1, 1, 1,
0, -1, 1,
-1, -1, 0,
-1, -1, 1,
0, -1, 1,
1, -1, 0,
1, -1, 1,
0, -1, -1,
1, -1, 0,
1, -1, -1,
0, -1, -1,
-1, -1, 0,
-1, -1, -1,
};

static const unsigned int CUBE_VERTICES[] = {
	0, 0, 0,
	1, 0, 0,
	1, 1, 0,
	0, 1, 0,

	0, 0, 1,
	1, 0, 1,
	1, 1, 1,
	0, 1, 1
};

chunk* chunk_new(int xpos, int ypos, int zpos);

void chunk_generate(chunk* this);

void chunk_render(chunk* this);

#define block_or_default(chunk, x, y, z, def)  (block_in_bounds(x, y, z) ? chunk_block_at(chunk, x, y, z) : def)

#endif