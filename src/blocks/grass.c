#include "block.h"

void grass_init() {
	block_names[GRASS] = "Grass";
	set_atlas_coord(GRASS, NORTH, 0, 0);
	set_atlas_coord(GRASS, SOUTH, 0, 0);
	set_atlas_coord(GRASS, EAST, 0, 0);
	set_atlas_coord(GRASS, WEST, 0, 0);
	set_atlas_coord(GRASS, TOP, 1, 0);
	set_atlas_coord(GRASS, BOTTOM, 2, 0);

}
