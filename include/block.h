#ifndef BLOCK_H
#define BLOCK_H

#include "common.h"
#include <cglm/ivec2.h>
typedef struct {
	unsigned int atlas_coordinates[12];
	char* name;
} block_data;

#define AIR 0
#define GRASS 1
#define DIRT 2
#define NUM_BLOCKS 3

#define set_atlas_coord(block, direction, x, y)  { \
	atlas_coordinates[direction][block * 2] = x;	\
	atlas_coordinates[direction][block * 2 + 1] = y; \
}

#define get_atlas_coord(block, direction) {atlas_coordinates[direction][block * 2], atlas_coordinates[direction][block * 2 + 1]}
#define get_block_name(block) block_names[block]

extern char* block_names[NUM_BLOCKS];

extern int atlas_coordinates[6][NUM_BLOCKS * 2];

#define _BLOCK_DECL(_name)\
    extern void _name##_init();\
    _name##_init();

static inline void block_init() {
	_BLOCK_DECL(air)
	_BLOCK_DECL(grass)
	_BLOCK_DECL(dirt)
}


#endif