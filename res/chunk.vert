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
const uint chunk_size = 32u;
out float shade;

float normal_shades[6] = float[6] (
 0.8f,
 0.85f,
 0.9f,
 0.87f,
 1.0f,
 0.6f
);

float ao_shades[4] = float[4] (
0.4f,
0.6f,
0.8f,
1.0f
);

vec2 texCoords[4] = vec2[4] (

    vec2(0.0,0.0),
    vec2(1.0,0.0),
    vec2(1.0,1.0),
    vec2(0.0,1.0)
);



void main()
{

	uint index = (vertex_data & 0xC0000000u) >> 30u;

	uint pos_x = (uint(chunk_pos.x) * chunk_size) + ((vertex_data & 1056964608u) >> 24u);

	uint pos_y = (uint(chunk_pos.y) * chunk_size) + ((vertex_data & 0xFC0000u) >> 18u);

	uint pos_z = (uint(chunk_pos.z) * chunk_size) + ((vertex_data & 0x3F000u) >> 12u);

	float sprite_x = float((vertex_data & 0xF00u) >> 8u);
	float sprite_y = float((vertex_data & 0xF0u) >> 4u);

	normal = (lighting_data & 0x7u);
	ao = (lighting_data & 0x18u) >> 3u;

	uint sky_light = (lighting_data & 0x1E0u) >> 5u;
	uint block_light = (lighting_data & 0x1E00u) >> 9u;

	float sky_light_brightness = 0.25f + 0.05f * float(sky_light);
	float block_light_brightness = 0.25f + 0.05f * float(block_light);
	float light_level = max(sky_light_brightness, block_light_brightness);

	shade = normal_shades[normal] * ao_shades[ao] * light_level;
	float sprite_size = 16.f / 256.f;

	texCoord = texCoords[index] * sprite_size + vec2(sprite_x * sprite_size, sprite_y * sprite_size);

   gl_Position = projection * view * vec4(pos_x, pos_y, pos_z, 1);
}