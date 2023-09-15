// minecraftc.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include "common.h"
#include "io.h"
#include "vbo.h"
#include "vao.h"
#include "texture.h"
#include "shader.h"
#include "chunk.h"
#include "block.h"
#include <time.h>
#include "skybox.h"
#include <math.h>


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    camera_handle_mouse_movement(state.cam, (float)xposIn, (float)yposIn);
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    state.cam->viewport_height = height;
    state.cam->viewport_width = width;
}


int main()
{ 
	if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!state.window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(state.window);
    glfwSetCursorPosCallback(state.window, mouse_callback);
    glfwSetWindowSizeCallback(state.window, window_resize_callback);
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }


    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    block_init();
    skybox_init();
    shader* chunk_shader = shader_create("res/chunk.frag", "res/chunk.vert");

    /* Loop until the user closes the window */
    state.delta_time = 0.016f;
    state.cam = camera_new(64, 0.1, 100);

    chunk* chunk = chunk_new(0, 0, 0);
    chunk_build_mesh(chunk);

    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    bool exit = false;

    state.ticks = 0;
    state.tps = 20;
    
    clock_t tick_timer = clock();
    clock_t time = clock();
    texture* atlas = texture_from_file("res/atlas.png");
    state.sky_light = 15;
    int light_level = state.sky_light;
    shader_uniform_texture(chunk_shader, "atlas", atlas, 0);
    while (!glfwWindowShouldClose(state.window) && !exit)
    {
        if (tick_timer + CLOCKS_PER_SEC / state.tps < clock()) {
            state.ticks++;
            
            tick_timer = clock();
        }
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox_render();
        
        camera_handle_input(state.cam);
        //printf("(%f, %f, %f) %s\n", state.cam->look.x, state.cam->look.y, state.cam->look.z, cardinal_directions[camera_get_cardinal_direction(state.cam)]);
        shader_use(chunk_shader);
        shader_uniform_view_proj(chunk_shader, camera_get_view_projection(state.cam));
        if (light_level != state.sky_light) {
            light_level = state.sky_light;
            chunk_build_lighting(chunk);
        }
        chunk_render(chunk);

        if (glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            exit = true;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(state.window);

        /* Poll for and process events */
        glfwPollEvents();
        state.delta_time = (float)(clock() - time) / (float)CLOCKS_PER_SEC;
        //printf("%f\n", state.delta_time);
        time = clock();
    }

    glfwTerminate();
    return 0;
}

