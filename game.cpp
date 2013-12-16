#include "game.h"
#include "shader.h"
#include "transform.h"
#include "cube.h"
#include "font.h"
#include <iostream>

Performance perf;

State game_state = PlayState;

Shader 
	default_shader,
	diffuse_shader,
	sprite_shader;

mat4 
	mat_perspective,
	mat_orthographic,
	mat_model,
	mat_view;

Mesh 
	grid,
	cube;

Font 
	debug_font,
	game_font;

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
	sprite_shader.load("shaders/sprite.vs", "shaders/sprite.fs");

	if (!load_font(debug_font, "textures/proggytinyttsz_8x12.png") ||
		!load_font(game_font, "textures/segoe_script_36x53.png"))
		return false;

	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / float(height);

	mat_perspective = glm::perspective(45.0f, ratio, 0.5f, 8.0f);
	mat_orthographic = glm::ortho(0.0f, float(width), float(height), 0.0f);
	mat_view = mat4(1.0f);
	mat_model = mat4(1.0f);

	grid = generate_grid(8, 1.0f, 1.0f, 1.0f, 1.0f);
	cube = generate_color_cube(1.0f, 1.0f, 0.23f, 0.21f);

	return true;
}

void unload_game()
{
	delete_mesh(grid);
	delete_mesh(cube);
	delete_font(debug_font);
	delete_font(game_font);
	default_shader.dispose();
	diffuse_shader.dispose();
	sprite_shader.dispose();
}

void update_menu(GLFWwindow *window, double dt)
{

}

void update_play(GLFWwindow *window, double dt)
{
	float time = (float) glfwGetTime();
	mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-0.35f) * rotate_y(time);
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
	double time = glfwGetTime();

	glClearColor(0.13f, 0.13f, 0.13f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	default_shader.use();

	default_shader.set_uniform("projection", mat_perspective);
	default_shader.set_uniform("view", mat_view);

	mat_model = scale(1.0f);
	default_shader.set_uniform("model", mat_model);
	render_pos_col(GL_LINES, default_shader, grid);

	mat_model = scale(0.2f);
	default_shader.set_uniform("model", mat_model);
	render_pos_col(GL_TRIANGLES, default_shader, cube);

	default_shader.unuse();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	sprite_shader.use();
	sprite_shader.set_uniform("projection", mat_orthographic);
	sprite_shader.set_uniform("view", mat4(1.0f));

	string perf_str;
	perf_str += "update: " + std::to_string(int(perf.update_time * 1000.0)) + "ms\n";
	perf_str += "render: " + std::to_string(int(perf.render_time * 1000.0)) + "ms";

	draw_string(debug_font, sprite_shader, 5.0f, 5.0f, perf_str);
	sprite_shader.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
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