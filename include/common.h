#ifndef COMMON
#define COMMON

#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"

struct {
	GLFWwindow* window;
	float delta_time;
	camera* cam;

} state;

static const char* cardinal_directions[6] = {
	"NORTH (+Z)",
	"SOUTH (-Z)",
	"EAST (+X)",
	"WEST (-X)",
	"TOP (+Y)",
	"BOTTOM (-Y)"
};

#endif