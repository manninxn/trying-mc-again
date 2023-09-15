#include "block.h"

void dirt_init() {
	block_names[DIRT] = "Dirt";
	set_atlas_coord(DIRT, NORTH, 2, 0);
	set_atlas_coord(DIRT, SOUTH, 2, 0);
	set_atlas_coord(DIRT, EAST, 2, 0);
	set_atlas_coord(DIRT, WEST, 2, 0);
	set_atlas_coord(DIRT, TOP, 2, 0);
	set_atlas_coord(DIRT, BOTTOM, 2, 0);

}
