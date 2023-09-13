#ifndef VAO_H
#define VAO_H

#include <memory.h>
#include <glad/glad.h>

#include "common.h"
#include "vbo.h"

typedef struct {
	unsigned int id;
} vao;

vao* vao_new();

void vao_bind(vao* this);

void vao_attribute(vao* this, vbo* vbo, int index, size_t size, GLenum type, int stride, int offset);

#endif