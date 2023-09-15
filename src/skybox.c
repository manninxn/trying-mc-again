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


void stars_init() {
	skybox.stars.shader = shader_create("res/stars.frag", "res/stars.vert");

	for (int i = 0; i < NUM_STARS; i++) {
		float d;// = asinf(rand() / (float)RAND_MAX) * 2.0f * PI - PI;
		//gaussian(0, HALF_PI, &d);
		d = asinf(rand() / (float)RAND_MAX * 2.f - 1.f) * (rand() % 2 ? -1.f : 1.f);
		//printf("%f\n", d);
		float a = (rand() / (float)RAND_MAX) * PI * 2 - PI;
		vec3 star = { d, a, (rand() / (float)RAND_MAX) * 4.f };
		memcpy(skybox.stars.positions[i], star, sizeof(float) * 3);
	}
	skybox.stars.vao = vao_new();
	skybox.stars.vbo = vbo_new(GL_ARRAY_BUFFER, false);
	vao_attribute(skybox.stars.vao, skybox.stars.vbo, 0, 3, GL_FLOAT, sizeof(float) * 3, 0);
	vbo_buffer_data(skybox.stars.vbo, skybox.stars.positions, sizeof(float) * 3 * NUM_STARS);

}


void sun_moon_init() {
	skybox.sun_and_moon.shader = shader_create_with_geom("res/sun.frag", "res/sun.vert", "res/sun.geom");
	skybox.sun_and_moon.vao = vao_new();
	skybox.sun_and_moon.vbo = vbo_new(GL_ARRAY_BUFFER, false);
	vao_attribute(skybox.sun_and_moon.vao, skybox.sun_and_moon.vbo, 0, 1, GL_INT, sizeof(int), 0);
	int sam[] = {0, 1};
	vbo_buffer_data(skybox.sun_and_moon.vbo, sam, sizeof(int) * 2);
	skybox.sun_and_moon.texture = texture_from_file("res/sun.png");
	
}


void sun_moon_render() {
	shader_use(skybox.sun_and_moon.shader);
	vp viewproj = camera_get_view_projection(state.cam);

	float angle = remap((float)(state.ticks % 2400), 0.f, 2400.f, 0.f, 360.f);
	glm_make_rad(&angle);

	vec3 axis = { 0, 0, 1 };
	glm_rotate(viewproj.view.raw, angle, axis);

	shader_uniform_float(skybox.sun_and_moon.shader, "time", angle);
	//vec3 up = viewproj.view.
	shader_uniform_view_proj(skybox.sun_and_moon.shader, viewproj);
	shader_uniform_texture(skybox.sun_and_moon.shader, "tex", skybox.sun_and_moon.texture, 1);
	vbo_bind(skybox.sun_and_moon.vbo);
	vao_bind(skybox.sun_and_moon.vao);
	glDrawArrays(GL_POINTS, 0, 2);
}

void stars_render() {
	shader_use(skybox.stars.shader);
	vp viewproj = camera_get_view_projection(state.cam);

	float angle = remap((float)(state.ticks % 2400), 0.f, 2400.f, 0.f, 360.f);
	//printf("%d\n", state.ticks);
	glm_make_rad(&angle);

	vec3 axis = { 0, 0, 1 };
	glm_rotate(viewproj.view.raw, angle, axis);
	mat4 up;
	glm_mat4_identity(up);
	glm_rotate(up, angle, axis);
	//vec3 upvec = { up.m01, viewproj.view.m11, viewproj.view.m21 };
	axis[0] = 1;
	axis[2] = 0;
	vec3 upvec = { up[0][1], up[1][1], up[2][1] };
	float dot = glm_vec3_dot(axis, upvec);
	//printf("%f\n", dot);
	shader_uniform_float(skybox.stars.shader, "time", dot);
	//vec3 up = viewproj.view.
	shader_uniform_view_proj(skybox.stars.shader, viewproj);

	vbo_bind(skybox.stars.vbo);
	vao_bind(skybox.stars.vao);
	glDrawArrays(GL_POINTS, 0, NUM_STARS);
	
}


void background_init() {
	skybox.background.shader = shader_create("res/background.frag", "res/background.vert");
	skybox.background.vao = vao_new();
	skybox.background.vbo = vbo_new(GL_ARRAY_BUFFER, false);
	vao_attribute(skybox.background.vao, skybox.background.vbo, 0, 1, GL_UNSIGNED_INT, sizeof(int), 0);
	int verts[6] = {1, 3, 0, 1, 2, 3 };
	vbo_buffer_data(skybox.background.vbo, verts,6 * sizeof(int));

}

void background_render() {
	shader_use(skybox.background.shader);

	float angle = remap((float)(state.ticks % 2400), 0.f, 2400.f, 0.f, 360.f);
	glm_make_rad(&angle);
	vec3 axis = { 0, 0, 1 };

	mat4 up;
	glm_mat4_identity(up);
	glm_rotate(up, angle, axis);
	//vec3 upvec = { up.m01, viewproj.view.m11, viewproj.view.m21 };
	axis[0] = 1;
	axis[2] = 0;
	vec3 upvec = { up[0][1], up[1][1], up[2][1] };
	float dot = glm_vec3_dot(axis, upvec);
	printf("%f\n", dot);
	shader_uniform_float(skybox.background.shader, "time", dot);

	vbo_bind(skybox.background.vbo);
	vao_bind(skybox.background.vao);
	glDrawArrays(GL_TRIANGLES, 0, 6 );
}

void skybox_init() {
	background_init();
	stars_init();
	sun_moon_init();
}

void skybox_render() {
	background_render();
	glClear(GL_DEPTH_BUFFER_BIT);

	stars_render();
	glClear(GL_DEPTH_BUFFER_BIT);

	sun_moon_render();
	glClear(GL_DEPTH_BUFFER_BIT);

}