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
	long long ticks;
	int tps;
	int sky_light;

} state;
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define TOP 4
#define BOTTOM 5

#define remap(value, low1, high1, low2, high2) ((low2) + ((value) - (low1)) * ((high2) - (low2)) / ((high1) - (low1)))

static const char* cardinal_directions[6] = {
	"NORTH (+Z)",
	"SOUTH (-Z)",
	"EAST (+X)",
	"WEST (-X)",
	"TOP (+Y)",
	"BOTTOM (-Y)"
};

#endif