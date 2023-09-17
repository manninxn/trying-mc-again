
#define FNL_IMPL
#include "chunk.h"
#include <stdio.h>
#include "block.h"
#include <memory.h>

chunk* chunk_new(int xpos, int ypos, int zpos) {
	chunk* this = malloc(sizeof(chunk));

	this->pos[0] = xpos;
	this->pos[1] = ypos;
	this->pos[2] = zpos;

	this->vert_vbo = vbo_new(GL_ARRAY_BUFFER, true);
	this->lighting_vbo = vbo_new(GL_ARRAY_BUFFER, true);
	this->ebo = vbo_new(GL_ELEMENT_ARRAY_BUFFER, true);
	this->vao = vao_new();
	this->num_elems = 0;
	vao_attribute(this->vao, this->vert_vbo, 0, 1, GL_UNSIGNED_INT, sizeof(int), 0);
	vao_attribute(this->vao, this->lighting_vbo, 1, 1, GL_UNSIGNED_INT,  sizeof(int), 0);
	this->build_lighting = true;
	this->build_mesh = true;
	this->mesh_regen_ready = false;
	this->lighting_regen_ready = false;
	this->mutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	return this;
}

//x = index / (size * size)
// y = (index / size) % size
// 
// z = index % size


unsigned int build_vert_data(int x, int y, int z, int block_id, int direction, int index) {
	//printf(" Added vertex (%d, %d, %d)\n", x, y, z);
	ivec2 atlas_coords = { atlas_coordinates[direction][block_id * 2], atlas_coordinates[direction][block_id * 2 + 1] };
 	return (z << 12u) | (y << 18u) | (x << 24u) | (index << 30u) | (atlas_coords[0] << 8u) | (atlas_coords[1] << 4u);
}

unsigned int build_lighting_data(int normal_index, int ao_value, int sky_light_level, int block_light_level) {
	return (normal_index) | (ao_value << 3u) | (sky_light_level << 5u) | (block_light_level << 9u);
}
int vertex_ao(side1, side2, corner) {
	if (side1 && side2) {
		return 0;
	}
	return 3 - (side1 + side2 + corner);
}

void chunk_build_lighting(chunk* this) {
	this->lighting_data = malloc(this->num_verts * sizeof(unsigned int));
	unsigned int vert_index = 0;
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int x = i / (CHUNK_SIZE * CHUNK_SIZE);
		int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
		int z = i % CHUNK_SIZE;
		unsigned char block = this->blocks[i];
		if (block == 0) continue;
		for (int j = 0; j < 6; j++) {
			ivec3 adjacent;
			glm_ivec3_add((ivec3) { x, y, z }, faces[j], adjacent);
			if ((block_in_bounds(adjacent[0], adjacent[1], adjacent[2]) ? chunk_block_at(this, adjacent[0], adjacent[1], adjacent[2]) : 0) != 0) continue;
			int aos[4];
			for (int vert = 0; vert < 4; vert++) {

				int index = j * 4 + vert;
				const unsigned int* ao_vals = &AO_NEIGHBORS[index * 9];

				bool side1 = block_or_default(this, x + ao_vals[0], y + ao_vals[1], z + ao_vals[2], false);
				bool side2 = block_or_default(this, x + ao_vals[3], y + ao_vals[4], z + ao_vals[5], false);
				bool corner = block_or_default(this, x + ao_vals[6], y + ao_vals[7], z + ao_vals[8], false);

				int ao = vertex_ao(side1, side2, corner);
				aos[vert] = build_lighting_data(j, ao, state.sky_light, 0);
			}
			for (int vert = 0; vert < 4; vert++) {
				int index = vert;
				if (aos[0] + aos[2] > aos[1] + aos[3]) {

					index = (vert + 3) % 4;
				}

				this->lighting_data[vert_index] = aos[index];
				vert_index++;
			}
		}
	}
	
	
	
	this->build_lighting = false;
	this->lighting_regen_ready = true;
}

void chunk_generate(chunk* this, noise_params* noise) {
	// pass 1: set blocks
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int x = this->pos[0] * CHUNK_SIZE + i / (CHUNK_SIZE * CHUNK_SIZE);
		int y = this->pos[1] * CHUNK_SIZE + (i / CHUNK_SIZE) % CHUNK_SIZE;
		int z = this->pos[2] * CHUNK_SIZE + i % CHUNK_SIZE;
		float height = 1.0f + fnlGetNoise2D(&noise->main, x, z);
		//this->blocks[i] = (y - 5) < (height * 1.5f);
		int dirt_or_grass = fnlGetNoise3D(&noise->main, x, y + 1, z) > 0 ? DIRT : GRASS;
		this->blocks[i] = fnlGetNoise3D(&noise->main, x, y, z) > 0 ? dirt_or_grass : AIR;
		unsigned char block = this->blocks[i];
	}
	this->is_generated = true;
	printf("Generated chunk (%d, %d, %d)\n", this->pos[0], this->pos[1], this->pos[2]);
}


void chunk_build_mesh(chunk* this) {

	this->mesh_data = malloc(BLOCKS_PER_CHUNK * sizeof(unsigned int) * 4 * 6);
	
	this->mesh_indices_data = malloc(BLOCKS_PER_CHUNK * sizeof(unsigned int) * 6 * 6);
	
	unsigned int num_verts = 0;
	unsigned int num_elems = 0;
	int index_offset = 0;
	// pass 2: build mesh
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int x = i / (CHUNK_SIZE * CHUNK_SIZE);
		int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
		int z = i % CHUNK_SIZE;
		unsigned char block = this->blocks[i];
		if (block == 0) continue;
		
		for (int j = 0; j < 6; j++) {
			ivec3 adjacent;
			glm_ivec3_add((ivec3) { x, y, z }, faces[j], adjacent);
			if ((block_in_bounds(adjacent[0], adjacent[1], adjacent[2]) ? chunk_block_at(this, adjacent[0], adjacent[1], adjacent[2]) : 0) != 0) continue;
			int aos[4];
			int v[4];
			for (int vert = 0; vert < 4; vert++) {

				int index = j * 4 + vert;
				const unsigned int* ao_vals = &AO_NEIGHBORS[index * 9];

				bool side1 = block_or_default(this, x + ao_vals[0], y + ao_vals[1], z + ao_vals[2], false);
				bool side2 = block_or_default(this, x + ao_vals[3], y + ao_vals[4], z + ao_vals[5], false);
				bool corner = block_or_default(this, x + ao_vals[6], y + ao_vals[7], z + ao_vals[8], false);

				int ao = vertex_ao(side1, side2, corner);
				aos[vert] = ao;
				const unsigned int* vertex = &CUBE_VERTICES[CUBE_INDICES[(j * 6) + UNIQUE_INDICES[vert]] * 3];
				v[vert] = build_vert_data(vertex[0] + x, vertex[1] + y, vertex[2] + z, block, j, vert);
			}
			for (int vert = 0; vert < 4; vert++) {
				int index = vert;
				if (aos[0] + aos[2] > aos[1] + aos[3]) {
				
					index = (vert + 3) % 4;
				}

				this->mesh_data[num_verts] = v[index];
				num_verts++;
			}
			for (int index = 0; index < 6; index++) {
				this->mesh_indices_data[num_elems] = index_offset + FACE_INDICES[index];
				num_elems++;
			}
			index_offset += 4;
		}

	}

	
	this->num_elems = num_elems;
	this->num_verts = num_verts;

	this->build_mesh = false;
	this->mesh_regen_ready = true;
	
}

void chunk_finalize_mesh_build(chunk* this) {
	vbo_buffer_data(this->ebo, this->mesh_indices_data, sizeof(int) * this->num_elems);
	vbo_buffer_data(this->vert_vbo, this->mesh_data, sizeof(int) * this->num_verts);
	free(this->mesh_data);
	free(this->mesh_indices_data);
	this->mesh_regen_ready = false;
}

void chunk_finalize_lighting_build(chunk* this) {
	vbo_buffer_data(this->lighting_vbo, this->lighting_data, sizeof(int) * this->num_verts);
	free(this->lighting_data);
	this->lighting_regen_ready = false;
}

void chunk_update(chunk* this) {

	if (this->mesh_regen_ready) {
		chunk_finalize_mesh_build(this);
	}
	if (this->lighting_regen_ready) {
		chunk_finalize_lighting_build(this);
	}
}


void chunk_render(chunk* this) {
	vao_bind(this->vao);
	vbo_bind(this->ebo);
	glDrawElements(GL_TRIANGLES, this->num_elems, GL_UNSIGNED_INT, 0);
}