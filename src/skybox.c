#include "skybox.h"

void gaussian(float mu, float sigma, float* a) {
	float epsilon = FLT_EPSILON;
	float two_pi = 2.0f * M_PI;

	//create two random numbers, make sure u1 is greater than epsilon
	double u1, u2;
	do
	{
		u1 = (rand() / (float)RAND_MAX);
	} while (u1 <= epsilon);
	u2 = (rand() / (float)RAND_MAX);

	//compute z0 and z1
	float mag = sigma * sqrtf(-2.0f * logf(u1));
	*a = mag * cosf(two_pi * u2) + mu;
	float b = mag * sinf(two_pi * u2) + mu;
	//srand(b);
}

void skybox_init() {
	skybox.shader = shader_create("res/skybox.frag", "res/skybox.vert");
	
	for (int i = 0; i < NUM_STARS; i++) {
		float d;// = asinf(rand() / (float)RAND_MAX) * 2.0f * PI - PI;
		//gaussian(0, HALF_PI, &d);
		d = asinf(rand() / (float)RAND_MAX * 2.f - 1.f) * (rand() % 2 ? -1.f : 1.f);
		printf("%f\n", d);
		float a = (rand() / (float)RAND_MAX) * PI * 2 - PI;
		vec3 star = { d, a, 8.f };
		memcpy(skybox.stars[i], star, sizeof(float) * 3);
	}
	skybox.vao = vao_new();
	skybox.vbo = vbo_new(GL_ARRAY_BUFFER, false);
	vao_attribute(skybox.vao, skybox.vbo, 0, 3, GL_FLOAT, sizeof(float) * 3, 0);
	vbo_buffer_data(skybox.vbo, skybox.stars, sizeof(float) * 3 * NUM_STARS);

}

void skybox_render() {
	shader_use(skybox.shader);
	vp viewproj = camera_get_view_projection(state.cam);
	
	float angle = remap((float)(state.ticks % 2400), 0.f, 2400.f, 0.f, 360.f);
	glm_make_rad(&angle);
	
	vec3 axis = { 0, 0, 1 };
	glm_rotate(viewproj.view.raw, angle, axis);
	shader_uniform_view_proj(skybox.shader, viewproj);

	vbo_bind(skybox.vbo);
	vao_bind(skybox.vao);
	glDrawArrays(GL_POINTS, 0, NUM_STARS);
	glClear(GL_DEPTH_BUFFER_BIT);

}