#version 330 core
layout (location = 0) in uint vertex_data;
layout (location = 1) in uint lighting_data;
uniform ivec3 chunk_pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
flat out uint normal;
flat out uint ao;
const uint chunk_size = 32;
out float shade;

float normal_shades[6] = {
 0.8f,
 0.85f,
 0.9f,
 0.87f,
 1.0f,
 0.3f
};

float ao_shades[4] = {
0.75f,
0.85f,
0.95f,
1.0f,
};

void main()
{

	uint tex_coordinate = (vertex_data & 0xC000000u) >> 30u;

	uint pos_x = (vertex_data & 1056964608u) >> 24u;

	uint pos_y = (vertex_data & 0xFC0000u) >> 18u;

	uint pos_z = (vertex_data & 0x3F000u) >> 12u;

	uint atlas_x = (vertex_data & 0xF00u) >> 8u;
	uint atlas_y = (vertex_data & 0xF0u) >> 4u;

	normal = (lighting_data & 0x7u);
	ao = (lighting_data & 0x18u) >> 3u;
	shade = normal_shades[normal] * ao_shades[ao];
   gl_Position = projection * view * vec4(pos_x, pos_y, pos_z, 1);
}