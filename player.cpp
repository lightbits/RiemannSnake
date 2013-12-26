#include "player.h"
#include "cube.h"
#include "transform.h"
#include <iostream>

struct Player
{
	vec4 head_color;
	vec4 body_color;
	vec2 tan_vel;
	vec3 vel;
	vec3 pos;
	Spherical sphere_pos;
	float speed;

	Mesh mesh_head;
};

Player player;

Spherical to_spherical(float rho, const vec3 &p)
{
	Spherical spos;
	if (p.z == 0 && p.y > 0)
		spos.theta = M_PI_TWO;
	else if (p.z == 0 && p.y < 0)
		spos.theta = -M_PI_TWO;
	else
		spos.theta = atan2(p.x, p.z);
	spos.phi = asin(p.y / rho);
	spos.rho = rho;
	return spos;	
}

vec3 to_cartesian(const Spherical &p)
{
	float r = p.rho * cos(p.phi);
	return vec3(
		r * sin(p.theta),
		p.rho * sin(p.phi),
		r * cos(p.theta));
}

void tangent_frame(const vec3 &pos, const vec3 &vel, vec3 &T, vec3 &N, vec3 &B)
{
	T = glm::normalize(vel);
	N = glm::normalize(pos);
	B = glm::normalize(glm::cross(N, T));
}

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
	player.sphere_pos = Spherical(1.0f, 0.0f, 0.3f);
	player.speed = player_speed;
	player.tan_vel = vec2(0.0f, 1.0f);
	player.head_color = head_color;
	player.body_color = body_color;

	player.vel = vec3(0.0f, 1.0f, 0.0f);
	player.pos = vec3(0.0f, 0.0f, 1.0f);

	player.mesh_head = generate_sphere(1.0f, 8, 8, head_color);
}

void free_player(GLFWwindow *window)
{
	delete_mesh(player.mesh_head);
}

void set_player_velocity(float x, float y)
{
	player.tan_vel = vec2(x, y);
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
	vec2 tan_vel(0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		tan_vel = glm::normalize(vec2(-0.1f, 1.0f));
	else if (glfwGetKey(window, GLFW_KEY_LEFT))
		tan_vel = glm::normalize(vec2(+0.1f, 1.0f));

	vec3 T, N, B;
	tangent_frame(player.pos, player.vel, T, N, B);
	player.vel = glm::normalize(tan_vel.x * B + tan_vel.y * T);
		
	//vec2 residual_vel = player.tan_vel;
	//float acceleration = 0.0f;
	//if (glfwGetKey(window, GLFW_KEY_LEFT))
	//	acceleration = -2.0f;
	//else if (glfwGetKey(window, GLFW_KEY_RIGHT))
	//	acceleration = 2.0f;

	//residual_vel = rotate_xy(residual_vel, acceleration * dt);
	//set_player_velocity(residual_vel.x, residual_vel.y);
}

void update_player(GLFWwindow *window, double dt)
{
	player.vel -= level_get_normal(player.pos) * player.speed * player.speed * float(dt) / 1.0f;
	player.pos += player.vel * player.speed * float(dt);
	player.sphere_pos = to_spherical(1.0f, player.pos);
	//player.sphere_pos = to_spherical(1.0f, world);
	//vec3 acceleration = -glm::normalize(player.pos) * player.speed * player.speed / 1.0f;
	//player.vel += acceleration * float(dt);
	//move_spherical(&player.pos, player.vel * player.speed * float(dt));
}

void render_player(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	//vec3 world_pos = level_to_world_pos(player.pos);
	mat4 transform = translate(player.pos) * scale(0.05f);
	shader->set_uniform("model", transform);
	render_colored(GL_TRIANGLES, *shader, player.mesh_head);

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

Spherical player_get_sphere_pos()
{
	return player.sphere_pos;
}

vec3 player_get_world_pos()
{
	return player.pos;
}

vec2 player_get_tan_vel()
{
	return player.tan_vel;
}

vec3 player_get_world_vel()
{
	return player.vel;
}

int player_get_length()
{
	return 0;
	//return player.blocks.size();
}