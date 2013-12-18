#include "util.h"
#include "game.h"
#include "timer.h"
#include <iostream>
#define CURSOR_MODE GLFW_CURSOR_DISABLED
#define VSYNC 1
#define FSAA_SAMPLES 4
#define GL_VERSION_MAJOR 3
#define GL_VERSION_MINOR 1
#define GL_PROFILE 0 // 0 is auto
#define RESIZABLE_WINDOW GL_FALSE

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

	glfwWindowHint(GLFW_OPENGL_PROFILE, GL_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_SAMPLES, FSAA_SAMPLES);
	glfwWindowHint(GLFW_RESIZABLE, RESIZABLE_WINDOW);

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

		init_game(window);

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float) height;
		glViewport(0, 0, width, height);

		int updates_per_sec = 60;
		double target_frame_time = 1.0 / 60.0;
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

			double update_time = glfwGetTime() - now;
			now = glfwGetTime();

			render_game();
			glfwSwapBuffers(window);
			glfwPollEvents();

			double render_time = glfwGetTime() - now;
			if (render_time < target_frame_time)
				sleep(target_frame_time - render_time);

			if (check_gl_errors(std::cerr))
			{
				std::cin.get();
				glfwSetWindowShouldClose(window, GL_TRUE);
			}

			// Update global performance info
			perf.render_time = render_time;
			perf.update_time = update_time;
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