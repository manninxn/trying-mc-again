#include "shader.h"

shader* shader_create(const char* fragment_path, const char* vertex_path) {
	shader* this = malloc(sizeof(shader));

	char* fs = load_file(fragment_path);
	char* vs = load_file(vertex_path);

	int frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_handle, 1, &fs, NULL);
	glCompileShader(frag_handle);

	int vert_handle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_handle, 1, &vs, NULL);
	glCompileShader(vert_handle);

	this->id = glCreateProgram();
	glAttachShader(this->id, frag_handle);
	glAttachShader(this->id, vert_handle);

	glLinkProgram(this->id);
	shader_use(this);
	glDeleteShader(frag_handle);
	glDeleteShader(vert_handle);

	return this;
}

void shader_use(shader* this) {
	glUseProgram(this->id);
}

void shader_uniform_float(shader* this, char* name, float value) { glUniform1f(glGetUniformLocation(this->id, name), value); }

void shader_uniform_vec2(shader* this, char* name, vec2 value) { glUniform2f(glGetUniformLocation(this->id, name), value[0], value[1]); }

void shader_uniform_vec3(shader* this, char* name, vec3 value) { glUniform3f(glGetUniformLocation(this->id, name), value[0], value[1], value[2]); }

void shader_uniform_vec4(shader* this, char* name, vec4 value) { glUniform4f(glGetUniformLocation(this->id, name), value[0], value[1], value[2], value[3]); }

void shader_uniform_int(shader* this, char* name, int value) { glUniform1i(glGetUniformLocation(this->id, name), value); }

void shader_uniform_ivec2(shader* this, char* name, ivec2 value) { glUniform2i(glGetUniformLocation(this->id, name), value[0], value[1]); }

void shader_uniform_ivec3(shader* this, char* name, ivec3 value) { glUniform3i(glGetUniformLocation(this->id, name), value[0], value[1], value[2]); }

void shader_uniform_ivec4(shader* this, char* name, ivec4 value) { glUniform4i(glGetUniformLocation(this->id, name), value[0], value[1], value[2], value[3]); }

void shader_uniform_mat2(shader* this, char* name, mat2 value) { glUniformMatrix2fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, value); }

void shader_uniform_mat3(shader* this, char* name, mat3 value) { glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, value); }

void shader_uniform_mat4(shader* this, char* name, mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, (float*)value);
}

void shader_uniform_view_proj(shader* this, vp vp) {
	shader_uniform_mat4(this, "view", vp.view.raw);
	shader_uniform_mat4(this, "projection", vp.projection.raw);
}


//void shader_uniform_texture(shader* this, char* name, struct Texture texture, GLuint n) {
//	glActiveTexture(GL_TEXTURE0 + n);
//	texture_bind(texture);
//	glUniform1i(glGetUniformLocation(this->id, (const GLchar*)name), n);
//}