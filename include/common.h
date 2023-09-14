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
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define TOP 4
#define BOTTOM 5

static const char* cardinal_directions[6] = {
	"NORTH (+Z)",
	"SOUTH (-Z)",
	"EAST (+X)",
	"WEST (-X)",
	"TOP (+Y)",
	"BOTTOM (-Y)"
};

#endif