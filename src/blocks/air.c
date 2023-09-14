#include "block.h"

void air_init() {
	block_names[GRASS] = "Air";
	set_atlas_coord(GRASS, NORTH, 0, 0);
	set_atlas_coord(GRASS, SOUTH, 0, 0);
	set_atlas_coord(GRASS, EAST, 0, 0);
	set_atlas_coord(GRASS, WEST, 0, 0);
	set_atlas_coord(GRASS, TOP, 0, 0);
	set_atlas_coord(GRASS, BOTTOM, 0, 0);
}
