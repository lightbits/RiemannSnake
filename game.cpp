#include "player.h"
#include "level.h"
#include "util.h"
#include "types.h"
#include "shader.h"
#include "font.h"
#include "text.h"
#include "transform.h"
#include <iostream>

#define LEVEL_SIZE 12
#define PLAYER_START_LENGTH 4
#define GRID_SCALE 2.0f
#define GRID_COLOR 48 / 255.0f, 98 / 255.0f, 48 / 255.0f
#define PLAYER_HEAD_COLOR 15 / 255.0f, 56 / 255.0f, 15 / 255.0f
#define PLAYER_BODY_COLOR 48 / 255.0f, 98 / 255.0f, 48 / 255.0f
#define BG_COLOR 155 / 255.0f, 188 / 255.0f, 15 / 255.0f, 1.0f
#define TEXT_COLOR 15 / 255.0f, 56 / 255.0f, 15 / 255.0f

enum GameState { PLAY_STATE, MENU_STATE };
GameState game_state = PLAY_STATE;

Shader 
	shader_default,
	shader_sprite;

Font
	font_debug;

mat4
	mat_perspective,
	mat_orthographic,
	mat_view;

bool load_game(GLFWwindow *window)
{
	if(!load_player(window) ||
		!load_level(window) ||
		!shader_default.load("shaders/default.vs", "shaders/default.fs") ||
		!shader_sprite.load("shaders/sprite.vs", "shaders/sprite.fs") ||
		!load_font(font_debug, "textures/proggytinyttsz_8x12.png"))
		return false;
	return true;
}

void init_game(GLFWwindow *window)
{
	init_player(window, PLAYER_START_LENGTH, vec3(PLAYER_HEAD_COLOR), vec3(PLAYER_BODY_COLOR));
	init_level(window, LEVEL_SIZE, GRID_SCALE, vec3(GRID_COLOR));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	mat_perspective = glm::perspective(45.0f, width / float(height), 0.5f, 8.0f);
	mat_orthographic = glm::ortho(0.0f, float(width), float(height), 0.0f);
	mat_view = mat4(1.0f);
}

void free_game(GLFWwindow *window)
{
	free_player(window);
	free_level(window);
}

void on_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void update_play_state(GLFWwindow *window, double dt)
{
	handle_player_input(window, dt);
	update_player(window, dt);
	update_level(window, dt);

	float time = (float) glfwGetTime();
	mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-1.54f);
	//mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-0.45f) * rotate_y(sin(time) * 0.01f + 0.3f);
}

void update_menu_state(GLFWwindow *window, double dt)
{
	
}

void render_play_state(GLFWwindow *window, double dt)
{
	double time = glfwGetTime();

	glClearColor(BG_COLOR);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_default.use();
	shader_default.set_uniform("projection", mat_perspective);
	shader_default.set_uniform("view", mat_view);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);
	render_player(window, dt);
	render_level(window, dt);
	glDisable(GL_DEPTH_TEST);

	shader_default.unuse();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	shader_sprite.use();
	shader_sprite.set_uniform("projection", mat_orthographic);
	shader_sprite.set_uniform("view", scale(2.0f));

	Text debug_text;
	debug_text << "x: " << player_get_pos().x << "\ny: " << player_get_pos().y << '\n';
	debug_text << "x: " << player_get_vel().x << "\ny: " << player_get_vel().y;
	draw_string(font_debug, shader_sprite, 5.0f, 5.0f, vec4(TEXT_COLOR, 1.0), debug_text.get_string());
	shader_sprite.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void render_menu_state(GLFWwindow *window, double dt)
{
	
}

void update_game(GLFWwindow *window, double dt)
{
	switch (game_state) 
	{
	case PLAY_STATE: update_play_state(window, dt); break;
	case MENU_STATE: update_menu_state(window, dt); break;
	}
}

void render_game(GLFWwindow *window, double dt)
{
	switch (game_state) 
	{
	case PLAY_STATE: render_play_state(window, dt); break;
	case MENU_STATE: render_menu_state(window, dt); break;
	}
}