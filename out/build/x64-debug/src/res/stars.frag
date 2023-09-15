#version 330

out vec4 FragColor;
uniform float time;

void main() {
	if(time < 0.05f) discard;
	FragColor = vec4(1.0f, 1.0f, 1.0f, time);
}