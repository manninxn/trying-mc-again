#include "chunk.h"
#include "world.h"

#include "common.h"


DWORD WINAPI chunk_meshing(LPVOID lpParam);

world* world_new(int render_distance) {
	world* this = malloc(sizeof(world));
	this->render_distance = render_distance;
	this->chunks = malloc(sizeof(chunk*) * render_distance * render_distance);
	this->loaded_position[0] = 0;
	this->loaded_position[1] = 0;
	this->loaded_position[2] = 0;
	this->loaded_chunks = 0;
	this->noise = malloc(sizeof(noise_params));
	this->noise->main = fnlCreateState();
	this->noise->main.noise_type = FNL_NOISE_OPENSIMPLEX2;
	this->noise->main.frequency = 0.05f;
	this->light_level = state.sky_light;
	this->chunk_access_mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	this->meshing_thread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)chunk_meshing,
		this,
		0,
		&this->meshing_thread_id);
	return this;
}

DWORD WINAPI chunk_meshing(LPVOID lpParam) {
		world* this = (world*)lpParam;
		HANDLE mutex = this->chunk_access_mutex;
		int i = 0;
	while (true) {


		DWORD dwCount = 0, result;

		if (this->loaded_chunks == 0) continue;
		// Request ownership of mutex.
		
		i %= this->loaded_chunks;

		chunk* chunk = this->chunks[i];
		if (chunk->build_mesh) {
			chunk_build_mesh(chunk);
		}
		if (chunk->build_lighting) {
			chunk_build_lighting(chunk);
		}
		i++;
		


					
	}
}

void world_generate_chunk(world* this, ivec3 chunk_coordinates) {
	DWORD result = WaitForSingleObject(
		this->chunk_access_mutex,    // handle to mutex
		INFINITE);  // no time-out interval

	chunk* chunk = chunk_new(chunk_coordinates[0], chunk_coordinates[1], chunk_coordinates[2]);
	CHUNK_AT(this, chunk_coordinates[0], chunk_coordinates[1], chunk_coordinates[2]) = chunk;
	chunk_generate(chunk, this->noise);
	this->loaded_chunks++;

	ReleaseMutex(this->chunk_access_mutex);
}

void world_update(world* this) {





	bool rebuild_lighting = false;

	if (this->light_level != state.sky_light) {
		this->light_level = state.sky_light;
		rebuild_lighting = true;
	}
	for (int i = 0; i < this->loaded_chunks; i++) {


		chunk* chunk = this->chunks[i];

		chunk->build_lighting |= rebuild_lighting;
		chunk_update(chunk);


	}

}

void world_render(world* this, shader* shader) {

	for (int i = 0; i < this->loaded_chunks; i++) {
		chunk* chunk = this->chunks[i];
		
		shader_uniform_ivec3(shader, "chunk_pos", (ivec3){chunk->pos[0], chunk->pos[1], chunk->pos[2] });
		chunk_render(chunk);
	}
}

void world_update_loaded_position(world* this, ivec3 new_pos) {

}
