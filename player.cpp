#include "player.h"
#include "cube.h"
#include "transform.h"

struct PlayerBlock
{
	Mesh mesh_cube;
	vec2 pos;
	vec2 vel;
};

struct Player
{
	std::vector<PlayerBlock> blocks;
	vec3 head_color;
	vec3 body_color;
	vec2 pos;
	vec2 vel;
	float speed;
};

enum class PlayerState { IDLE, MOVING };

Player player;
PlayerState player_state = PlayerState::IDLE;

PlayerBlock create_block(float size,
						 const vec2 &p0, 
						 const vec2 &v0, 
						 const vec3 &color)
{
	PlayerBlock block;
	block.mesh_cube = generate_color_cube(size, color);
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
				 float player_speed,
				 const vec2 &start_pos,
				 const vec3 &head_color, 
				 const vec3 &body_color)
{
	player.pos = start_pos;
	player.speed = player_speed;
	player.vel = vec2(1.0f, 0.0f) * player_speed;
	player.head_color = head_color;
	player.body_color = body_color;

	player.blocks.push_back(create_block(
		1.0f,
		player.pos,
		player.vel, 
		head_color));

	for (int i = 1; i < start_length; ++i)
		player.blocks.push_back(create_block(
		1.0f,
		player.blocks[i - 1].pos + vec2(0.0f, 1.0f),
		player.blocks[i - 1].vel, body_color));
}

void free_player(GLFWwindow *window)
{
	for (PlayerBlock &block : player.blocks)
		delete_mesh(block.mesh_cube);
}

void set_player_velocity(float x, float y, float speed)
{
	player.speed = speed;
	player.vel = vec2(x, y) * speed;
}

void handle_player_input(GLFWwindow *window, double dt)
{
	if (glfwGetKey(window, GLFW_KEY_LEFT))
		set_player_velocity(-1.0f, 0.0f, player.speed);
	else if (glfwGetKey(window, GLFW_KEY_RIGHT))
		set_player_velocity(+1.0f, 0.0f, player.speed);

	if (glfwGetKey(window, GLFW_KEY_UP))
		set_player_velocity(0.0f, -1.0f, player.speed);
	else if (glfwGetKey(window, GLFW_KEY_DOWN))
		set_player_velocity(0.0f, +1.0f, player.speed);
}

void move(vec2 *position, vec2 velocity)
{
	int size = level_get_size();
	vec2 to = *position + velocity;
	if (to.x >= size) to.x = 0.0f;
	if (to.y >= size) to.y = 0.0f;
	if (to.x < 0) to.x = size - 1.0f;
	if (to.y < 0) to.y = size - 1.0f;
	*position = to;
}

// void add_block()

void increase_length()
{
	player.blocks.push_back(create_block(
		1.0f,
		player.blocks[player.blocks.size() - 1].pos - player.blocks[player.blocks.size() - 1].vel,
		player.blocks[player.blocks.size() - 1].vel, 
		player.body_color));
}

void update_player(GLFWwindow *window, double dt)
{
	move(&player.pos, player.vel * float(dt));
	player.blocks[0].vel = player.vel;
	move(&player.blocks[0].pos, player.blocks[0].vel * float(dt));
	for (int i = player.blocks.size() - 1; i > 0; --i)
	{
		move(&player.blocks[i].pos, player.blocks[i].vel * float(dt));
		player.blocks[i].vel = player.blocks[i - 1].vel;
	}
		
	//int tile_x = int(player.pos.x);
	//int tile_y = int(player.pos.y);
	//switch (level_get_tile(tile_x, tile_y))
	//{
	//case LevelTile::OBSTACLE:
	//	// Player loses
	//	break;
	//case LevelTile::APPLE:
	//	increase_length();
	//	break;
	//}
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

vec2 player_get_pos()
{
	return player.pos;
}

vec2 player_get_vel()
{
	return player.vel;
}

int player_get_length()
{
	return player.blocks.size();
}