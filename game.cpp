#include "player.h"
#include "level.h"
#include "util.h"
#include "types.h"
#include "shader.h"
#include "font.h"
#include "text.h"
#include "transform.h"
#include <iostream>

const int		PLAYER_START_LENGTH = 1;
const float		PLAYER_SPEED = 1.0f;
const float		LEVEL_RADIUS = 1.0f;
const uint32	LEVEL_EDGE_COLOR = 0xFF2222FF;
const uint32	LEVEL_FILL_COLOR = 0x222328FF;
const vec3		PLAYER_START_POS = vec3(LEVEL_RADIUS, M_PI_TWO, 0.0f);
const uint32	PLAYER_HEAD_COLOR = 0xB22763FF;
const uint32	PLAYER_BODY_COLOR = 0xFF529CFF;
const uint32	BACKGROUND_COLOR = 0x2DB3CCFF;
const uint32	TEXT_COLOR = 0xFFF59FFF;

vec4 to_rgb(uint32 hex)
{
	return vec4(
		((hex>>24) & 0xff) / 255.0f,
		((hex>>16) & 0xff) / 255.0f,
		((hex>>8) & 0xff) / 255.0f,
		(hex & 0xff) / 255.0f);
}

enum GameState { PLAY_STATE, MENU_STATE };
GameState game_state = PLAY_STATE;

Shader 
	shader_default,
	shader_sprite,
	shader_wireframe;

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
		!shader_wireframe.load("shaders/wireframe.vs", "shaders/wireframe.fs") ||
		!load_font(font_debug, "textures/proggytinyttsz_8x12.png"))
		return false;
	return true;
}

void init_game(GLFWwindow *window)
{
	init_player(window, 
		PLAYER_START_LENGTH, 
		PLAYER_SPEED,
		PLAYER_START_POS,
		to_rgb(PLAYER_HEAD_COLOR),
		to_rgb(PLAYER_BODY_COLOR));

	init_level(window,
		LEVEL_RADIUS,
		to_rgb(LEVEL_EDGE_COLOR),
		to_rgb(LEVEL_FILL_COLOR));

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
	//mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-1.54f);
	mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-0.45f) * rotate_y(time * 0.2f);
}

void update_menu_state(GLFWwindow *window, double dt)
{
	
}

void render_play_state(GLFWwindow *window, double dt)
{
	double time = glfwGetTime();

	vec4 cc = to_rgb(BACKGROUND_COLOR);
	glClearColor(cc.r, cc.g, cc.b, cc.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);

	shader_wireframe.use();
	shader_wireframe.set_uniform("projection", mat_perspective);
	shader_wireframe.set_uniform("view", mat_view);
	render_level(window, dt);
	render_player(window, dt);
	shader_wireframe.unuse();

	shader_default.use();
	shader_default.set_uniform("projection", mat_perspective);
	shader_default.set_uniform("view", mat_view);
	
	shader_default.unuse();
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	shader_sprite.use();
	shader_sprite.set_uniform("projection", mat_orthographic);
	shader_sprite.set_uniform("view", scale(1.0f));

	Text debug_text;
	debug_text << "x: " << player_get_pos().x << "\ny: " << player_get_pos().y << '\n';
	debug_text << "x: " << player_get_vel().x << "\ny: " << player_get_vel().y;
	draw_string(font_debug, shader_sprite, 5.0f, 5.0f, to_rgb(TEXT_COLOR), debug_text.get_string());
	shader_sprite.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
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