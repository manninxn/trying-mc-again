#version 330 core

in vec2 TexCoord;
uniform sampler2D tex;
flat in int index;
out vec4 FragColor;
in float valid;
void main() {
	vec4 color = texture(tex, TexCoord);
	//if(color.a < 0.1) discard;
	FragColor = color;
}