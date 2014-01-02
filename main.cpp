#include "util.h"
#include "game.h"
#include "timer.h"
#include "fileio.h"
#include <iostream>
#include <fstream>
#include <sstream>

void error_callback(int error, const char* description)
{
	log_msg(description);
}

void read_config(int &gl_version_major, 
				 int &gl_version_minor, 
				 int &fsaa_samples, 
				 int &window_width, 
				 int &window_height)
{
	// Defaults
	gl_version_major = 3;
	gl_version_minor = 1;
	fsaa_samples = 4;
	window_width = 720;
	window_height = 480;

	string file;
	if (!read_file("./config.txt", file))
	{
		// File missing, write it
		std::ofstream config("./config.txt");
		config<<"gl_version_major: "<<gl_version_major<<std::endl;
		config<<"gl_version_minor: "<<gl_version_minor<<std::endl;
		config<<"fsaa_samples: "<<fsaa_samples<<std::endl;
		config<<"window_width: "<<window_width<<std::endl;
		config<<"window_height: "<<window_height<<std::endl;
		config.close();
	}
	else
	{
		std::stringstream ss(file);
		read_word(ss);
		gl_version_major = read_int(ss);
		read_word(ss);
		gl_version_minor = read_int(ss);
		read_word(ss);
		fsaa_samples = read_int(ss);
		read_word(ss);
		window_width = read_int(ss);
		read_word(ss);
		window_height = read_int(ss);
	}
}

void shutdown(int exit_code)
{
	dump_log();
	exit(exit_code);
}

int main(int argc, char **argv)
{
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
	{
		log_msg("Failed to initialize GLFW");
		shutdown(EXIT_FAILURE);
	}

	int gl_version_major, gl_version_minor, fsaa_samples, window_width, window_height;
	read_config(gl_version_major, gl_version_minor, fsaa_samples, window_width, window_height);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_version_minor);
	glfwWindowHint(GLFW_SAMPLES, fsaa_samples);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, GL_TRUE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    window = glfwCreateWindow(window_width, window_height, "Spherical Snake", NULL, NULL);
    if (!window)
    {
		log_msg("Failed to create window");
        glfwTerminate();
		shutdown(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, on_key);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Show window centered
	const GLFWvidmode *curr_vid_mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int screen_width = curr_vid_mode->width;
	int screen_height = curr_vid_mode->height;
	glfwSetWindowPos(window, screen_width / 2 - window_width / 2, screen_height / 2 - window_height / 2);
	glfwShowWindow(window);

	if(LoadFunctions() == LS_LOAD_FAILED)
	{
		log_msg("Failed to load OpenGL functions");
		glfwTerminate();
		shutdown(EXIT_FAILURE);
	}

	try
	{
		if(!load_game(window))
		{
			log_msg("Failed to load content");
			glfwTerminate();
			shutdown(EXIT_FAILURE);
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

			render_game(window, dt);
			glfwSwapBuffers(window);
			glfwPollEvents();

			//double render_time = glfwGetTime() - now;
			//if (render_time < target_frame_time)
			//	sleep(target_frame_time - render_time);

			if (check_gl_errors(get_log_stream()))
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
	catch (std::exception &e)
	{
		log_msg("An unexpected error occured: ");
		log_msg(e.what());
	}
    
    glfwDestroyWindow(window);
    glfwTerminate();
	shutdown(EXIT_SUCCESS);
}