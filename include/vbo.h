#ifndef VBO_H
#define VBO_H

#include <common.h>
#include <memory.h>
#include "glad/glad.h"
typedef struct {
	unsigned int id;
	GLenum target;
	bool dynamic;
} vbo;

vbo* vbo_new(GLenum target, bool dynamic);

void vbo_bind(vbo* this);

void vbo_buffer_data(vbo* this, void* data, size_t size);

#endif