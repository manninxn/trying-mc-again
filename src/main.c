// minecraftc.cpp : Defines the entry point for the application.
//

#include <stdio.h>
#include "common.h"
#include "io.h"
#include "vbo.h"
#include "vao.h"
#include "shader.h"
#include "chunk.h"
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    camera_handle_mouse_movement(state.cam, (float)xposIn, (float)yposIn);
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


    //glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    shader* shader = shader_create("res/frag.glsl", "res/vert.glsl");

    /* Loop until the user closes the window */
    state.delta_time = 0.016f;
    state.cam = camera_new(64, 0.1, 100);

    chunk* chunk = chunk_new(0, 0, 0);
    chunk_generate(chunk);

    glfwSetInputMode(state.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    bool exit = false;
    while (!glfwWindowShouldClose(state.window) && !exit)
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_handle_input(state.cam);
        //printf("(%f, %f, %f) %s\n", state.cam->look.x, state.cam->look.y, state.cam->look.z, cardinal_directions[camera_get_cardinal_direction(state.cam)]);
        shader_use(shader);
        shader_uniform_view_proj(shader, camera_get_view_projection(state.cam));
        chunk_render(chunk);

        if (glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            exit = true;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(state.window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
