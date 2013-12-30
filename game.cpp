#include "player.h"
#include "level.h"
#include "util.h"
#include "types.h"
#include "shader.h"
#include "font.h"
#include "text.h"
#include "cube.h"
#include "transform.h"
#include <iostream>

const float		PLAYER_SPEED = 0.5f;
const float		PLAYER_RADIUS = 0.05f;
const float		LEVEL_RADIUS = 0.7f;
const float		OBJECT_RADIUS = 0.05f;
const double	SPAWN_INTERVAL = 2.5;
const vec3		PLAYER_START_POS = vec3(0.0f, 0.0f, LEVEL_RADIUS);

const uint32	PLAYER_HEAD_COLOR = 0xFFFFFFFF;
const uint32	PLAYER_BODY_COLOR = 0xFFCCCCFF;

const uint32	LEVEL_EDGE_COLOR = 0xFFF59FFF;
const uint32	LEVEL_FILL_COLOR = 0xFF529CFF;

const uint32	BACKGROUND_COLOR = 0x2DB3CCFF;
const uint32	TEXT_COLOR = 0xFFFFFFFF;

enum GameState { PLAY_STATE, MENU_STATE, GAME_OVER_STATE };
GameState game_state = MENU_STATE;

Shader 
	shader_default,
	shader_sprite,
	shader_wireframe,
	shader_background;

Font
	font_debug;

mat4
	mat_perspective,
	mat_orthographic,
	mat_view;

int final_score;
int high_score;
double game_over_timer;

bool load_game(GLFWwindow *window)
{
	if(!load_player(window) ||
		!load_level(window) ||
		!shader_default.load("shaders/default.vs", "shaders/default.fs") ||
		!shader_sprite.load("shaders/sprite.vs", "shaders/sprite.fs") ||
		!shader_wireframe.load("shaders/wireframe.vs", "shaders/wireframe.fs") ||
		!shader_background.load("shaders/background.vs", "shaders/background.fs") ||
		!load_font(font_debug, "textures/proggytinyttsz_8x12.png"))
		return false;

	final_score = 0;
	high_score = 0;
	game_over_timer = 0.0;

	return true;
}

void init_game(GLFWwindow *window)
{
	init_level(window,
		LEVEL_RADIUS,
		OBJECT_RADIUS,
		SPAWN_INTERVAL,
		to_rgb(LEVEL_EDGE_COLOR),
		to_rgb(LEVEL_FILL_COLOR));

	init_player(window,  
		PLAYER_SPEED,
		PLAYER_RADIUS,
		PLAYER_START_POS,
		to_rgb(PLAYER_HEAD_COLOR),
		to_rgb(PLAYER_BODY_COLOR));

	vec2i size = get_window_size(window);
	mat_perspective = glm::perspective(45.0f, size.x / float(size.y), 0.5f, 8.0f);
	mat_orthographic = glm::ortho(0.0f, float(size.x), float(size.y), 0.0f);
	mat_view = mat4(1.0f);
}

void free_game(GLFWwindow *window)
{
	free_player(window);
	free_level(window);
	shader_default.dispose();
	shader_wireframe.dispose();
	shader_background.dispose();
	shader_sprite.dispose();
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

	if (player_is_dead())
	{
		final_score = player_get_length();
		if (final_score > high_score)
			high_score = final_score;
		free_player(window);
		free_level(window);
		init_game(window);

		game_over_timer = 1.0;
		game_state = GAME_OVER_STATE;
	}

	float time = float(glfwGetTime());
	vec3 pos = player_get_world_pos();
	vec3 n = level_get_normal(pos);
	mat_view = glm::lookAt(pos + n * 1.5f, pos, player_get_world_vel());
}

void update_menu_state(GLFWwindow *window, double dt)
{
	update_player(window, dt);
	float time = float(glfwGetTime());
	vec3 pos = player_get_world_pos();
	vec3 n = level_get_normal(pos);
	mat_view = glm::lookAt(pos + n * 1.5f, pos, player_get_world_vel());

	if (glfwGetKey(window, GLFW_KEY_LEFT) ||
		glfwGetKey(window, GLFW_KEY_RIGHT))
		game_state = PLAY_STATE;
}

void update_game_over_state(GLFWwindow *window, double dt)
{
	update_player(window, dt);
	float time = float(glfwGetTime());
	vec3 pos = player_get_world_pos();
	vec3 n = level_get_normal(pos);
	mat_view = glm::lookAt(pos + n * 1.5f, pos, player_get_world_vel());

	if (game_over_timer < 0.0)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT) ||
			glfwGetKey(window, GLFW_KEY_RIGHT))
		game_state = PLAY_STATE;
	}
	else
	{
		game_over_timer -= dt;
	}
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

	shader_default.use();
	shader_default.set_uniform("projection", mat_perspective);
	shader_default.set_uniform("view", mat_view);

	static Mesh sphere = generate_sphere(LEVEL_RADIUS, 32, 32, to_rgb(LEVEL_FILL_COLOR));
	shader_default.set_uniform("model", scale(0.9f));
	render_colored(GL_TRIANGLES, shader_default, sphere);

	render_player(window, dt);
	shader_default.unuse();

	shader_wireframe.use();
	shader_wireframe.set_uniform("projection", mat_perspective);
	shader_wireframe.set_uniform("view", mat_view);
	render_level(window, dt);
	shader_wireframe.unuse();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	shader_sprite.use();
	shader_sprite.set_uniform("projection", mat_orthographic);
	shader_sprite.set_uniform("view", scale(2.0f));
	shader_sprite.set_uniform("time", float(time));

	Text text;
	text << "Score: " << player_get_length();
	draw_string(font_debug, shader_sprite, 5.0f, 5.0f, to_rgb(TEXT_COLOR), text.get_string());
	shader_sprite.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}

void render_menu_state(GLFWwindow *window, double dt)
{
	double time = glfwGetTime();

	render_play_state(window, dt);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_TEXTURE_2D);

	shader_sprite.use();
	shader_sprite.set_uniform("projection", mat_orthographic);
	shader_sprite.set_uniform("view", scale(2.0f));
	shader_sprite.set_uniform("time", float(time));

	string text = "Press < or > to play!";
	vec2i win_size = get_window_size(window);
	draw_string(font_debug, 
		shader_sprite, 
		win_size.x / 4.0f, 
		win_size.y / 4.0f - 16.0f,
		to_rgb(TEXT_COLOR),
		text,
		true);
	shader_sprite.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void render_game_over_state(GLFWwindow *window, double dt)
{
	double time = glfwGetTime();

	render_play_state(window, dt);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_TEXTURE_2D);
	shader_sprite.use();
	shader_sprite.set_uniform("projection", mat_orthographic);
	shader_sprite.set_uniform("view", scale(2.0f));
	shader_sprite.set_uniform("time", float(time));

	Text text0;
	text0 << "Final score: " << final_score;
	Text text1;
	text1 << "Top score: " << high_score;
	Text text2;
	text2 << "Press < or > to play!";

	vec2i win_size = get_window_size(window);
	draw_string(font_debug, 
		shader_sprite, 
		win_size.x / 4.0f, 
		win_size.y / 4.0f - 16.0f,
		to_rgb(TEXT_COLOR),
		text2.get_string(),
		true);

	draw_string(font_debug, 
		shader_sprite, 
		win_size.x / 4.0f, 
		win_size.y / 4.0f - 32.0f,
		to_rgb(TEXT_COLOR),
		text1.get_string(),
		true);

	draw_string(font_debug, 
		shader_sprite, 
		win_size.x / 4.0f, 
		win_size.y / 4.0f - 48.0f,
		to_rgb(TEXT_COLOR),
		text0.get_string(),
		true);

	shader_sprite.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
}

void update_game(GLFWwindow *window, double dt)
{
	switch (game_state) 
	{
	case GAME_OVER_STATE: update_game_over_state(window, dt); break;
	case PLAY_STATE: update_play_state(window, dt); break;
	case MENU_STATE: update_menu_state(window, dt); break;
	}
}

void render_game(GLFWwindow *window, double dt)
{
	switch (game_state) 
	{
	case GAME_OVER_STATE: render_game_over_state(window, dt); break;
	case PLAY_STATE: render_play_state(window, dt); break;
	case MENU_STATE: render_menu_state(window, dt); break;
	}
}