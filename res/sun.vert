#version 330

layout (location = 0) in int i;

uniform float time;

uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
	vec3 cam_pos;
	mat4 view;
	mat4 proj;
	flat int index;
} vs_out;
out float valid;
void main() {
	vs_out.index = i;
	valid = i == 0 ? -1 : 1;
	float height = i == 0 ? 3.14159 : 0;
	float x = cos(0) * cos(height);
	float y = cos(0) * sin(height);
	float z = sin(0);

	vs_out.cam_pos =  vec3(view[0][3], view[1][3], view[2][3]);
	vs_out.view = view;
	vs_out.proj = projection;
	gl_PointSize = 16.f;
	gl_Position = vec4(x, y, z, 1.0f);
}