#include "game.h"

struct Cube
{
	float x, float y, float z;
	GLuint vbo;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

bool load_game()
{
	return true;
}

void update_game(double dt)
{

}

void render_game()
{
	glClear(GL_COLOR_BUFFER_BIT);
}