#include "game.h"
#include "shader.h"
#include "transform.h"
#include "cube.h"
#include "font.h"
#include <iostream>
#include "level.h"
#include "text.h"

Level level;

// Constants
const float		GRID_SIZE = 2.0f;
const int		LEVEL_SIZE = 12;
const float		CELL_SIZE = GRID_SIZE / float(LEVEL_SIZE);
const float		PLAYER_SCALE = CELL_SIZE / 2.0f;
const int		START_BLOCK_COUNT = 8;

vec2 level_to_world(const vec2i &p)
{
	return vec2(
		p.x * CELL_SIZE + CELL_SIZE / 2.0f - GRID_SIZE / 2.0f,
		p.y * CELL_SIZE + CELL_SIZE / 2.0f - GRID_SIZE / 2.0f);
}

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
	grid;

Font 
	debug_font,
	game_font;

struct PlayerBlock
{
	Mesh mesh;
	vec2i old_pos;
	vec2i pos;
	vec2i old_vel;
	vec2i vel;
};

struct Player
{
	std::vector<PlayerBlock> blocks;
	vec2i pos;
	vec2i vel;
};

Player player;

PlayerBlock create_block(const vec2i &p0, const vec2i &v0,
						 float r, float g, float b)
{
	PlayerBlock block;
	block.mesh = generate_color_cube(1.0f, r, g, b);
	block.old_pos = p0;
	block.old_vel = v0;
	block.pos = p0;
	block.vel = v0;
	return block;
}

void init_player()
{
	player.pos = vec2i(0, 0);
	player.vel = vec2i(0, 0);

	player.blocks.push_back(create_block(
		player.pos + vec2i(0, 1),
		player.vel, 
		1.0f, 0.23f, 0.21f));

	for (int i = 1; i < START_BLOCK_COUNT; ++i)
		player.blocks.push_back(create_block(
		player.blocks[i - 1].pos + vec2i(0, 1),
		player.blocks[i - 1].vel, 1.0f, 1.0f, 1.0f));
}

void delete_player()
{
	for (auto &block : player.blocks) {
		delete_mesh(block.mesh);
	}
}

void update_player(GLFWwindow *window, double dt)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		player.vel = vec2i(-1, 0);
	else if (glfwGetKey(window, GLFW_KEY_RIGHT))
		player.vel = vec2i(+1, 0);

	if (glfwGetKey(window, GLFW_KEY_UP))
		player.vel = vec2i(0, -1);
	else if (glfwGetKey(window, GLFW_KEY_DOWN))
		player.vel = vec2i(0, +1);
}

void update_player_position()
{
	player.pos += player.vel;

	std::vector<PlayerBlock> &blocks = player.blocks;
	blocks[0].old_pos = blocks[0].pos;
	blocks[0].old_vel = blocks[0].vel;

	blocks[0].pos = player.pos;
	blocks[0].vel = player.vel;
	for (int i = 1; i < player.blocks.size(); ++i)
	{
		blocks[i].old_pos = blocks[i].pos;
		blocks[i].pos = blocks[i - 1].old_pos;

		blocks[i].old_vel = blocks[i].vel;
		blocks[i].vel = blocks[i - 1].old_vel;
	}
}

void render_player()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0, 1.0);

	for (auto &block : player.blocks) 
	{
		vec2 world_pos = level_to_world(block.pos);
		mat4 transform = 
			translate(world_pos.x, PLAYER_SCALE / 2.0f, world_pos.y) *
			scale(PLAYER_SCALE);
		default_shader.set_uniform("model", transform);
		render_pos_col(GL_TRIANGLES, default_shader, block.mesh);
	}

	glDisable(GL_DEPTH_TEST);
}

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

void init_game(GLFWwindow *window)
{
	float ratio;
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / float(height);

	mat_perspective = glm::perspective(45.0f, ratio, 0.5f, 8.0f);
	mat_orthographic = glm::ortho(0.0f, float(width), float(height), 0.0f);
	mat_view = mat4(1.0f);
	mat_model = mat4(1.0f);

	level.init(LEVEL_SIZE);
	grid = generate_grid(LEVEL_SIZE, GRID_SIZE, 1.0f, 1.0f, 1.0f);
	init_player();
}

bool load_game(GLFWwindow *window)
{
	default_shader.load("shaders/default.vs", "shaders/default.fs");
	sprite_shader.load("shaders/sprite.vs", "shaders/sprite.fs");

	if (!load_font(debug_font, "textures/proggytinyttsz_8x12.png") ||
		!load_font(game_font, "textures/segoe_script_36x53.png"))
		return false;

	return true;
}

void unload_game()
{
	delete_mesh(grid);
	delete_player();
	delete_font(debug_font);
	delete_font(game_font);
	default_shader.dispose();
	diffuse_shader.dispose();
	sprite_shader.dispose();
}

void update_menu(GLFWwindow *window, double dt)
{

}

int gametick = 25;
int gametick_acc = 0;
void update_play(GLFWwindow *window, double dt)
{
	float time = (float) glfwGetTime();
	mat_view = translate(0.0f, 0.0f, -3.0f) * rotate_x(-0.45f) * rotate_y(sin(time) * 0.01f + 0.3f);

	update_player(window, dt);

	if (gametick_acc++ >= gametick)
	{
		update_player_position();
		gametick_acc = 0;
	}
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

	default_shader.set_uniform("model", mat4(1.0f));
	render_pos_col(GL_LINES, default_shader, grid);

	render_player();

	default_shader.unuse();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	sprite_shader.use();
	sprite_shader.set_uniform("projection", mat_orthographic);
	sprite_shader.set_uniform("view", mat4(1.0f));

	Text debug_text;
	debug_text << "update: " << int(perf.update_time * 1000) << "ms\n";
	debug_text << "render: " << int(perf.render_time * 1000) << "ms\n";
	debug_text << "x: " << player.pos.x << "\ny: " << player.pos.y;
	draw_string(debug_font, sprite_shader, 5.0f, 5.0f, debug_text.get_string());
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