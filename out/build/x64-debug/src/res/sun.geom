#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;



out vec2 TexCoord; 
in VS_OUT {
    vec3 cam_pos;
    mat4 view;
    mat4 proj;
    flat int index;
} gs_in[];

float size = 0.5f;

vec2 texCoords[4] = vec2[4] (

    vec2(1.0,1.0),
    vec2(1.0,0.0),
    vec2(0.0,1.0),
    vec2(0.0,0.0)
);


float sprite_size = 0.5f;

#define make_vert(index)         gl_Position = projection * mat4(mat3(view)) * plane_positions[index]; TexCoord = texCoords[index] * vec2(sprite_size, 1) + vec2(float(i) * sprite_size, 0); EmitVertex()

void main() {    
    vec3 V = normalize(vec3(gl_in[0].gl_Position) - gs_in[0].cam_pos);
    vec3 U = cross(vec3(0, 1, 0), normalize(V));
    vec3 W = cross(normalize(V), U);
    vec3 P0 = vec3(gl_in[0].gl_Position);

    vec4 plane_positions[4] = vec4[4] (
        normalize(vec4(P0 + (size) * U + (size) * W, 1.0f)),
        normalize(vec4(P0 + (size) * U - (size) * W, 1.0f)),
        normalize(vec4(P0 - (size) * U + (size) * W, 1.0f)),
        normalize(vec4(P0 - (size) * U - (size) * W, 1.0f))
    );

    mat4 projection = gs_in[0].proj;
    mat4 view = gs_in[0].view;

    int i = gs_in[0].index;
    make_vert(3);
    make_vert(2);
    make_vert(1);
    make_vert(0);
    //make_vert(2);
   // make_vert(1);

    EndPrimitive();
} 