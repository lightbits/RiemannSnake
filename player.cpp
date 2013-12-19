#include "player.h"
#include "cube.h"
#include "transform.h"

struct PlayerBlock
{
	Mesh mesh_cube;
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

PlayerBlock create_block(float size,
						 const vec2i &p0, 
						 const vec2i &v0, 
						 const vec3 &color)
{
	PlayerBlock block;
	block.mesh_cube = generate_color_cube(size, color);
	block.old_pos = p0;
	block.old_vel = v0;
	block.pos = p0;
	block.vel = v0;
	return block;
}

bool load_player(GLFWwindow *window)
{
	return true;
}

void init_player(GLFWwindow *window, 
				 int start_length, 
				 const vec3 &head_color, 
				 const vec3 &body_color)
{
	player.pos = vec2i(0, 0);
	player.vel = vec2i(0, 0);

	player.blocks.push_back(create_block(
		1.0f,
		player.pos,
		player.vel, 
		head_color));

	for (int i = 1; i < start_length; ++i)
		player.blocks.push_back(create_block(
		1.0f,
		player.blocks[i - 1].pos + vec2i(0, 1),
		player.blocks[i - 1].vel, body_color));
}

void free_player(GLFWwindow *window)
{
	for (PlayerBlock &block : player.blocks)
		delete_mesh(block.mesh_cube);
}

void handle_player_input(GLFWwindow *window, double dt)
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

void update_player(GLFWwindow *window, double dt)
{

}

void render_player(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();

	for (auto &block : player.blocks) 
	{
		vec2 world_pos = level_to_world_pos(block.pos);
		mat4 transform = 
			translate(world_pos.x, level_get_cell_size() / 2.0f, world_pos.y) *
			scale(level_get_cell_size());
		shader->set_uniform("model", transform);
		render_pos_col(GL_TRIANGLES, *shader, block.mesh_cube);
	}
}

vec2i player_get_pos()
{
	return player.pos;
}

vec2i player_get_vel()
{
	return player.vel;
}

int player_get_length()
{
	return player.blocks.size();
}