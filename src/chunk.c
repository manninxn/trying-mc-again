#include "chunk.h"
#include <stdio.h>
#include "block.h"
#include <memory.h>
#define FNL_IMPL
#include "noise.h"

chunk* chunk_new(int xpos, int ypos, int zpos) {
	chunk* this = malloc(sizeof(chunk));

	this->pos[0] = xpos;
	this->pos[1] = ypos;
	this->pos[2] = zpos;

	this->vbo = vbo_new(GL_ARRAY_BUFFER, true);
	this->ebo = vbo_new(GL_ELEMENT_ARRAY_BUFFER, true);
	this->vao = vao_new();
	this->num_elems = 0;
	vao_attribute(this->vao, this->vbo, 0, 1, GL_UNSIGNED_INT, 2 * sizeof(int), 0);
	vao_attribute(this->vao, this->vbo, 1, 1, GL_UNSIGNED_INT, 2 * sizeof(int), sizeof(int));
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

unsigned int build_lighting_data(int normal_index, int ao_value) {
	return (normal_index) | (ao_value << 3u);
}
int vertex_ao(side1, side2, corner) {
	if (side1 && side2) {
		return 0;
	}
	return 3 - (side1 + side2 + corner);
}
void chunk_generate(chunk* this) {
	unsigned int* verts = malloc(BLOCKS_PER_CHUNK * sizeof(unsigned int) * 4 * 6 * 2);
	unsigned int* elements = malloc(BLOCKS_PER_CHUNK * sizeof(unsigned int) * 6 * 6);
	
	
	unsigned int num_verts = 0;
	unsigned int num_elems = 0;
	int index_offset = 0;
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.frequency = 0.05f;
	

	// pass 1: set blocks
	for (int i = 0; i < BLOCKS_PER_CHUNK; i++) {
		int x = i / (CHUNK_SIZE * CHUNK_SIZE);
		int y = (i / CHUNK_SIZE) % CHUNK_SIZE;
		int z = i % CHUNK_SIZE;
		float height = 1.0f + fnlGetNoise2D(&noise, x, z);
		//this->blocks[i] = (y - 5) < (height * 1.5f);
		this->blocks[i] = fnlGetNoise3D(&noise, x, y, z) > 0;
		unsigned char block = this->blocks[i];
	}
	noise.frequency = 0.1f;
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
				//printf("\n\nINDEX: %d\nVERTEX: (%d, %d, %d) FACING %s\nSIDE 1: (%d, %d, %d)\n SIDE 2: (%d, %d, %d)\n CORNER: (%d, %d, %d)\n", index, vertex[0], vertex[1], vertex[2], cardinal_directions[j],
					//ao_vals[0], ao_vals[1], ao_vals[2],
					//ao_vals[3], ao_vals[4], ao_vals[5],
					//ao_vals[6], ao_vals[7], ao_vals[8]);

				bool side1 = block_or_default(this, x + ao_vals[0], y + ao_vals[1], z + ao_vals[2], false);
				bool side2 = block_or_default(this, x + ao_vals[3], y + ao_vals[4], z + ao_vals[5], false);
				bool corner = block_or_default(this, x + ao_vals[6], y + ao_vals[7], z + ao_vals[8], false);

				int ao = vertex_ao(side1, side2, corner);
				aos[vert] = build_lighting_data(j, ao);
				const unsigned int* vertex = &CUBE_VERTICES[CUBE_INDICES[(j * 6) + UNIQUE_INDICES[vert]] * 3];
				v[vert] = build_vert_data(vertex[0] + x, vertex[1] + y, vertex[2] + z, block, j, vert);
			}
			for (int vert = 0; vert < 4; vert++) {
				int index = vert;
				if (aos[0] + aos[2] > aos[1] + aos[3]) {
				
					index = (vert + 3) % 4;
				}

				verts[num_verts] = v[index];
				num_verts++;
				verts[num_verts] = aos[index];
				num_verts++;
			}
			for (int index = 0; index < 6; index++) {
				elements[num_elems] = index_offset + FACE_INDICES[index];
				num_elems++;
			}
			index_offset += 4;
		}

	}

	vbo_buffer_data(this->ebo, elements, sizeof(int) * num_elems);
	vbo_buffer_data(this->vbo, verts, sizeof(int) * num_verts);
	this->num_elems = num_elems;
	free(elements);
	free(verts);
}

void chunk_render(chunk* this) {
	vbo_bind(this->vbo);
	vbo_bind(this->ebo);
	vao_bind(this->vao);
	glDrawElements(GL_TRIANGLES, this->num_elems, GL_UNSIGNED_INT, 0);
}