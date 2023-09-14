#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
	unsigned int atlas_coordinates[12];
	char* name;
} block_data;

#define AIR 0
#define GRASS 1
#define NUM_BLOCKS 2

#define _BLOCK_DECL(_name)\
    extern void _name##_init();\
    _name##_init();

static inline void block_init() {
	_BLOCK_DECL(air);
	_BLOCK_DECL(grass)
}


#endif