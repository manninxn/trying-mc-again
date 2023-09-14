#ifndef SKYBOX_H
#define SKYBOX_H

#include "shader.h"
#include "vbo.h"
#include "vao.h"
#include "common.h"
#include "camera.h"
#include <math.h>
#include <cglm/vec3.h>

#define NUM_STARS 500

#define PI 3.14159f
#define HALF_PI 1.5708f
struct {
	shader* shader;
	vec3 stars[NUM_STARS];
	vao* vao;
	vbo* vbo;
} skybox;


void skybox_init();
void skybox_render();

#endif