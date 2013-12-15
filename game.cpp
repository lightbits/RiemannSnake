#include "game.h"
#include "shader.h"
#include "transform.h"

State game_state = PlayState;

Shader 
	default_shader,
	diffuse_shader;

mat4 
	mat_perspective,
	mat_model,
	mat_view;

void on_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	switch (game_state)
	{
	case MenuState:
		if (key == GLFW_KEY_P)
			game_state = PlayState;
		break;
	case PlayState:
		if (key == GLFW_KEY_M)
			game_state = MenuState;
		break;
	}
}

bool load_game(GLFWwindow *window)
{
	default_shader.load("shaders/default.vs", "shaders/default.fs");
	//diffuse_shader.load("shaders/diffuse.vs", "shaders/diffuse.fs");

	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	mat_perspective = glm::perspective(45.0f, ratio, 0.5f, 5.0f);
	mat_view = mat4(1.0f);
	mat_model = mat4(1.0f);

	return true;
}

void unload_game()
{
	default_shader.dispose();
	diffuse_shader.dispose();
}

void update_menu(GLFWwindow *window, double dt)
{

}

void update_play(GLFWwindow *window, double dt)
{
	float time = (float) glfwGetTime();
	mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_y(time);
}

void render_menu()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	default_shader.use();

	default_shader.set_uniform("projection", mat4(1.0));
	default_shader.set_uniform("model", mat4(1.0));
	default_shader.set_uniform("view", mat4(1.0));

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,		0.18f, 0.22f, 0.94f, 1.0f,
		+0.5f, -0.5f, 0.0f,		0.18f, 0.22f, 0.94f, 1.0f,
		+0.5f, +0.5f, 0.0f,		0.18f, 0.22f, 0.94f, 1.0f,
		-0.5f, +0.5f, 0.0f,		0.18f, 0.22f, 0.94f, 1.0f
	};

	uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

	GLuint vbo = gen_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	GLuint ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	default_shader.set_attribfv("position", 3, 7, 0);
	default_shader.set_attribfv("color", 4, 7, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	default_shader.unset_attrib("position");
	default_shader.unset_attrib("color");
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	default_shader.unuse();
}

void render_play()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	default_shader.use();

	default_shader.set_uniform("projection", mat_perspective);
	default_shader.set_uniform("model", mat_model);
	default_shader.set_uniform("view", mat_view);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,		0.95f, 0.22f, 0.18f, 1.0f,
		+0.5f, -0.5f, 0.0f,		0.95f, 0.22f, 0.18f, 1.0f,
		+0.5f, +0.5f, 0.0f,		0.95f, 0.22f, 0.18f, 1.0f,
		-0.5f, +0.5f, 0.0f,		0.95f, 0.22f, 0.18f, 1.0f
	};

	uint32 indices[] = { 0, 1, 2, 2, 3, 0 };

	GLuint vbo = gen_buffer(GL_ARRAY_BUFFER, sizeof(vertices), vertices);
	GLuint ibo = gen_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	default_shader.set_attribfv("position", 3, 7, 0);
	default_shader.set_attribfv("color", 4, 7, 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	default_shader.unset_attrib("position");
	default_shader.unset_attrib("color");
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	default_shader.unuse();
}

void update_game(GLFWwindow *window, double dt)
{
	switch(game_state)
	{
	case MenuState: update_menu(window, dt); break;
	case PlayState: update_play(window, dt); break;
	}
}

void render_game()
{
	switch (game_state)
	{
	case MenuState: render_menu(); break;
	case PlayState: render_play(); break;
	}
}