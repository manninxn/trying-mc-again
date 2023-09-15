#version 330
out vec4 FragColor;

vec4 day = vec4(0.62, 0.77, 1, 1);
vec4 night = vec4(0.06, .07, .09, 1);
uniform float time;
void main() {
	FragColor = mix(day, night, clamp(time * 4, 0, 1));
}