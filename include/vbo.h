#include <memory.h>
#include "glad/glad.h"
typedef struct {
	unsigned int id;
	GLenum target;
	char dynamic;
} vbo;

vbo* vbo_create(GLenum target, char dynamic) {
	vbo* vbo = malloc(sizeof(vbo));
	glGenBuffers(1, &vbo->id);
	vbo->target = target;
	vbo->dynamic = dynamic;
	return vbo;
}

void vbo_bind(vbo* this) {
	glBindBuffer(this->target, this->id);
}

void vbo_buffer_data(vbo* this, void* data, size_t size) {
	glBufferData(this->target, size, data, this->dynamic == 1 ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}