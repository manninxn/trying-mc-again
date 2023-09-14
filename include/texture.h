#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>
#include "io.h"
typedef struct {
	int handle;
	int width;
	int height;
	int channels;
} texture;

texture* texture_from_file(const char* file);
void texture_bind(texture* this);
void texture_delete(texture* this);
#endif