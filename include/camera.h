#ifndef CAMERA_H
#define CAMERA_H


#include <cglm/struct/vec3.h>
#include <cglm/cglm.h>
typedef struct {
	float fov;
	vec3s pos;
	vec3s look;
	float near_clip;
	float far_clip;
	float pitch, yaw;
	float last_x, last_y;
	float speed;
	float viewport_width, viewport_height;
} camera;

typedef struct {
	mat4s view;
	mat4s projection;
} vp;

vp camera_get_view_projection(camera* this);
camera* camera_new(float fov, float near_clip, float far_clip);
void camera_handle_input(camera* this);
void camera_handle_mouse_movement(camera* this, float xpos, float ypos);
int camera_get_cardinal_direction(camera* this);
#endif