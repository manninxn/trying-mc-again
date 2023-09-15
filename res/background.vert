#version 330
layout (location = 0) in uint index;

vec3 verts[4] = vec3[4] (
    vec3(-1.0,-1.0, 0.0),
    vec3(1.0,-1.0, 0.0),
    vec3(1.0,1.0, 0.0),
    vec3(-1.0,1.0, 0.0)
);

void main() {
    gl_Position = vec4(verts[index], 1.0f);
}