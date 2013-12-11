#include "util.h"
#include "game.h"
#include <iostream>
#define CURSOR_MODE GLFW_CURSOR_DISABLED
#define VSYNC 1

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

int main(int argc, char **argv)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    window = glfwCreateWindow(640, 480, "Spherical Snake", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
	glfwSwapInterval(VSYNC);
	glfwSetInputMode(window, GLFW_CURSOR, CURSOR_MODE);

	if(LoadFunctions() == LS_LOAD_FAILED)
	{
        fputs("Failed to load OpenGL functions", stderr);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

    if(!load_game())
    {
        fputs("Failed to load content", stderr);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int updates_per_sec = 25;
    double secs_per_update = 1.0 / double(updates_per_sec);
    double accumulator = 0.0;
    double dt = 0.0;
    double prev_now = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        double dt = now - prev_now;
        prev_now = now;

        accumulator += dt;
        while(accumulator >= secs_per_update)
        {
            update_game(secs_per_update);
            accumulator -= secs_per_update;
        }

        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);

        render_game();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}