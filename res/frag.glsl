#version 330 core
out vec4 FragColor;
uniform sampler2D tex;
in vec2 texCoord;
in float shade;


void main() {
	vec3 col = vec3(0.5f, 0.5f, 0.5f);
	FragColor = vec4(col * shade, 1.0f);
}