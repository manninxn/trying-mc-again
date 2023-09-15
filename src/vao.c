#include "vao.h"


vao* vao_new() {
	vao* vao = malloc(sizeof(vao));
	glGenVertexArrays(1, &vao->id);
	return vao;
}

void vao_bind(vao* this) {
	glBindVertexArray(this->id);
}

void vao_attribute(vao* this, vbo* vbo, int index, size_t size, GLenum type, int stride, int offset) {
	vao_bind(this);
	vbo_bind(vbo);
    switch (type) {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
        glVertexAttribIPointer(index, size, type, stride, (void*)offset);
        break;
    default:
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)offset);
        break;
    }
    glEnableVertexAttribArray(index);
}