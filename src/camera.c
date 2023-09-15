#include "camera.h"
#include "common.h"
vec3 view_up = { 0, 1, 0 };


camera* camera_new(float fov, float near_clip, float far_clip) {

	camera* this = malloc(sizeof(camera));
	this->far_clip = far_clip;
	this->near_clip = near_clip;
	this->fov = fov;
	this->pos = (vec3s) { 0, 8, 0 };
	this->look = (vec3s){ 1, 0, 1 };
	this->last_x = 0;
	this->last_y = 0;
	this->pitch = 0;
	this->yaw = 0;
	this->speed = 1.f;
	this->viewport_width = 1920;
	this->viewport_height = 1080;

	return this;


}

vp camera_get_view_projection(camera* this) {
	mat4s projection;
	glm_perspective(this->fov, (float)this->viewport_width / (float)this->viewport_height, this->near_clip, this->far_clip, &projection.raw);
	mat4s view;
	glm_look(this->pos.raw, this->look.raw, view_up, &view.raw);

	vp vp = {
		.projection = projection,
		.view = view
	};

	return vp;
}

void camera_handle_input(camera* this) {
	vec3 delta = GLM_VEC3_ZERO_INIT;

	float cam_speed = this->speed * 3 * state.delta_time;

	if (glfwGetKey(state.window, GLFW_KEY_W) == GLFW_PRESS) {
		vec3 out;
		glm_vec3_scale(this->look.raw, cam_speed, &out);
		glm_vec3_add(delta, out, &delta);
	}
	if (glfwGetKey(state.window, GLFW_KEY_S) == GLFW_PRESS) {
		vec3 out;
		glm_vec3_scale(this->look.raw, -cam_speed, &out);
		glm_vec3_add(delta, out, &delta);
	}

	if (glfwGetKey(state.window, GLFW_KEY_A) == GLFW_PRESS) {
		vec3 out;
		glm_cross(this->look.raw, view_up, &out);
		glm_normalize(&out);
		glm_vec3_scale(out, -cam_speed, &out);
		glm_vec3_add(delta, out, &delta);
	}

	if (glfwGetKey(state.window, GLFW_KEY_D) == GLFW_PRESS) {
		vec3 out;
		glm_cross(this->look.raw, view_up, &out);
		glm_normalize(&out);
		glm_vec3_scale(out, cam_speed, &out);
		glm_vec3_add(delta, out, &delta);
	}
	glm_vec3_add(this->pos.raw, delta, &this->pos.raw);

}

void camera_handle_mouse_movement(camera* this, float xpos, float ypos) {

	float xoffset = xpos - this->last_x;
	float yoffset = this->last_y - ypos; // reversed since y-coordinates go from bottom to top
	this->last_x = xpos;
	this->last_y = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	this->yaw += xoffset;
	this->pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (this->pitch > 89.0f)
		this->pitch = 89.0f;
	if (this->pitch < -89.0f)
		this->pitch = -89.0f;

	vec3 front;
	front[0] = cos(glm_rad(this->yaw)) * cos(glm_rad(this->pitch));
	front[1] = sin(glm_rad(this->pitch));
	front[2] = sin(glm_rad(this->yaw)) * cos(glm_rad(this->pitch));
	glm_vec3_normalize_to(front, &this->look.raw);

	glm_vec3_normalize(&this->look.raw);
	//printf("%f, %f, %f\n", this->look.x, this->look.y, this->look.z);
}


int camera_get_cardinal_direction(camera* this) {
	float max = max(max(fabs(this->look.x), fabs(this->look.y)), fabs(this->look.z));
	if (max == fabs(this->look.z)) {
		return this->look.z < 0;
	}
	else if (max == fabs(this->look.y)) {
		return this->look.y > 0 ? 4 : 5;
	}
	else {
		return this->look.x > 0 ? 2 : 3;
	}

}