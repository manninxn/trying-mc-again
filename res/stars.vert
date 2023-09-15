#version 330
layout (location = 0) in vec3 data;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 angle;

void main() {
	float x = cos(data.x) * cos(data.y);
	float y = cos(data.x) * sin(data.y);
	float z = sin(data.x);
	gl_Position = projection * mat4(mat3(view)) * vec4(x, y, z, 1.0f);
	gl_PointSize = data.z;
}