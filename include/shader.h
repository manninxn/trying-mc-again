#ifndef SHADER_H
#define SHADER_H
#include "common.h"
#include <glad/glad.h>
#include "io.h"
#include "texture.h"

typedef struct {
	unsigned int id;
} shader;


shader* shader_create(const char* fragment_path, const char* vertex_path);
void shader_use(shader* this);
shader* shader_create_with_geom(const char* fragment_path, const char* vertex_path, const char* geom_path);

void shader_uniform_float(shader* this, char* name, float value);
void shader_uniform_vec2(shader* this, char* name, vec2 value);
void shader_uniform_vec3(shader* this, char* name, vec3 value);
void shader_uniform_vec4(shader* this, char* name, vec4 value);
void shader_uniform_int(shader* this, char* name, int value);
void shader_uniform_ivec2(shader* this, char* name, ivec2 value);
void shader_uniform_ivec3(shader* this, char* name, ivec3 value);
void shader_uniform_ivec4(shader* this, char* name, ivec4 value);
void shader_uniform_mat2(shader* this, char* name, mat2 value);
void shader_uniform_mat3(shader* this, char* name, mat3 value);
void shader_uniform_mat4(shader* this, char* name, mat4 value);
void shader_uniform_texture(shader* this, char* name, texture* texture, GLuint n);
void shader_uniform_view_proj(shader* this, vp vp);
#endif