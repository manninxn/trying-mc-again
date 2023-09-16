#include "chunk.h"
#include "world.h"

world* world_new(int render_distance) {
	world* this = malloc(sizeof(world));
	this->render_distance = render_distance;
	this->loaded_chunks = malloc(sizeof(chunk*) * render_distance * render_distance);
	this->loaded_position[0] = 0;
	this->loaded_position[1] = 0;
	this->loaded_position[2] = 0;


	return this;
}

void world_update_loaded_position(world* this, vec3 new_pos) {

}
