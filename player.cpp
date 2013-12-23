#include "player.h"
#include "cube.h"
#include "transform.h"

struct Player
{
	vec4 head_color;
	vec4 body_color;
	vec3 pos;
	vec3 vel;
	float speed;

	Mesh mesh_head;
};

Player player;

bool load_player(GLFWwindow *window)
{
	return true;
}

void init_player(GLFWwindow *window, 
				 int start_length, 
				 float player_speed,
				 const vec3 &start_pos,
				 const vec4 &head_color, 
				 const vec4 &body_color)
{
	player.pos = start_pos;
	player.speed = player_speed;
	player.vel = vec3(0.0f, 0.0f, 1.0f) * player_speed;
	player.head_color = head_color;
	player.body_color = body_color;

	player.mesh_head = generate_wireframe_sphere(1.0f, 8, 8, head_color);
}

void free_player(GLFWwindow *window)
{
	delete_mesh(player.mesh_head);
}

void set_player_velocity(float r, float phi, float theta, float speed)
{
	player.speed = speed;
	player.vel = vec3(r, phi, theta) * speed;
}

void set_pos_spherical(vec3 *sp, float r, float phi, float theta)
{
	phi = glm::mod(phi, float(M_TWO_PI));
	theta = glm::mod(theta, float(M_TWO_PI));
	*sp = vec3(r, phi, theta);
}

void move_spherical(vec3 *sp, const vec3 &velocity)
{
	set_pos_spherical(sp, sp->x + velocity.x, sp->y + velocity.y, sp->z + velocity.z);
}

void handle_player_input(GLFWwindow *window, double dt)
{
	
	//if (glfwGetKey(window, GLFW_KEY_LEFT))

	//else if (glfwGetKey(window, GLFW_KEY_RIGHT))


	//if (glfwGetKey(window, GLFW_KEY_UP))

	//else if (glfwGetKey(window, GLFW_KEY_DOWN))

}

void update_player(GLFWwindow *window, double dt)
{
	move_spherical(&player.pos, player.vel * float(dt));
}

void render_player(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	vec3 world_pos = level_to_world_pos(player.pos.x, player.pos.y, player.pos.z);
	mat4 transform = translate(world_pos) * scale(0.1f);
	shader->set_uniform("model", transform);
	render_wireframe(GL_TRIANGLES, *shader, player.mesh_head);

	/*for (auto &block : player.blocks) 
	{
		vec2 world_pos = level_to_world_pos(block.pos);
		mat4 transform = 
			translate(world_pos.x, level_get_cell_size() / 2.0f, world_pos.y) *
			scale(level_get_cell_size());
		shader->set_uniform("model", transform);
		render_pos_col(GL_TRIANGLES, *shader, block.mesh_cube);
	}*/
}

vec3 player_get_pos()
{
	return player.pos;
}

vec3 player_get_vel()
{
	return player.vel;
}

int player_get_length()
{
	return 0;
	//return player.blocks.size();
}