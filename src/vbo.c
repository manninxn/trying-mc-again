#include "vbo.h"

vbo* vbo_new(GLenum target, bool dynamic) {
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
	vbo_bind(this);
	glBufferData(this->target, size, data, this->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}