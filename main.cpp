#include "util.h"
#include "game.h"
#include <iostream>
#define CURSOR_MODE GLFW_CURSOR_DISABLED
#define VSYNC 1

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
	std::cin.get();
}

int main(int argc, char **argv)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, 0); // 0 = auto
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(640, 480, "Spherical Snake", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, on_key);
	glfwSwapInterval(VSYNC);
	//glfwSetInputMode(window, GLFW_CURSOR, CURSOR_MODE);

	if(LoadFunctions() == LS_LOAD_FAILED)
	{
        fputs("Failed to load OpenGL functions", stderr);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	try
	{
		if(!load_game(window))
		{
			fputs("Failed to load content", stderr);
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);

		int updates_per_sec = 60;
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
			while (accumulator >= secs_per_update)
			{
				update_game(window, secs_per_update);
				accumulator -= secs_per_update;
			}

			now = glfwGetTime();
			render_game();

			glfwSwapBuffers(window);
			glfwPollEvents();
			double render_time = glfwGetTime() - now;

			/*if (render_time < target_frame_time)
				glfwSleep*/

			if (check_gl_errors(std::cerr))
			{
				std::cin.get();
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}
	}
	catch (std::exception &e)
	{
		std::cerr<<"An error occured: "<<e.what()<<std::endl;
		std::cin.get();
	}
    
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}