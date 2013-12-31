#include "player.h"
#include "cube.h"
#include "transform.h"
#include "noise.h"
#include "audio.h"
#include "level.h"
#include <iostream>

struct PlayerPart
{
	vec3 vel;
	vec3 pos;
	std::vector<vec3> past_pos;
	Mesh *mesh;
};

struct Player
{
	std::vector<PlayerPart> parts;
	vec4 head_color;
	vec4 body_color;
	float radius;
	float speed;
	std::vector<vec3> past_pos;
	vec3 pos;
	vec3 vel;
	bool dead;
};

Player player;
Mesh mesh_head;
Mesh mesh_body;

PlayerPart create_part(const vec3 &vel, const vec3 &pos, float radius, const vec4 &color, Mesh *mesh)
{
	PlayerPart part;
	part.vel = vel;
	part.pos = pos;
	part.mesh = mesh;
	return part;
}

void append_part()
{
	auto last = player.parts.end() - 1;
	vec3 p = last->pos;
	vec3 v = last->vel;
	player.parts.push_back(create_part(v, p, player.radius, player.body_color, &mesh_body));
}

bool load_player(GLFWwindow *window)
{
	return true;
}

void init_player(GLFWwindow *window, 
				 float player_speed,
				 float player_radius,
				 const vec3 &start_pos,
				 const vec4 &head_color, 
				 const vec4 &body_color)
{
	mesh_head = generate_sphere(1.0f, 8, 8, head_color);
	mesh_body = generate_sphere(1.0f, 8, 8, body_color);

	player.dead = false;
	player.speed = player_speed;
	player.radius = player_radius;
	player.head_color = head_color;
	player.body_color = body_color;

	player.vel = vec3(0.0f, 1.0f, 0.0f) * player_speed;
	player.pos = start_pos;

	player.parts.push_back(create_part(player.vel, player.pos, player.radius, player.head_color, &mesh_head));
}

void free_player(GLFWwindow *window)
{
	for (int i = 0; i < player.parts.size(); ++i)
		player.parts[i].past_pos.clear();
	player.parts.clear();
	player.past_pos.clear();

	delete_mesh(mesh_head);
	delete_mesh(mesh_body);
}

void handle_player_input(GLFWwindow *window, double dt)
{
	vec2 tan_vel(0.0f, 1.0f);
	if (glfwGetKey(window, GLFW_KEY_RIGHT))
		tan_vel = glm::normalize(vec2(-3.0f * dt, 1.0f));
	else if (glfwGetKey(window, GLFW_KEY_LEFT))
		tan_vel = glm::normalize(vec2(+3.0f * dt, 1.0f));

	vec3 n = level_get_normal(player.pos);
	vec3 t = glm::normalize(player.vel);
	vec3 b = glm::normalize(glm::cross(n, t));
	player.vel = glm::normalize(tan_vel.x * b + tan_vel.y * t) * player.speed;
}

extern void on_player_death();
extern void on_player_pickup();

void player_die()
{
	player.dead = true;

	// Give random velocities to parts
	for (auto &part : player.parts)
		part.vel = rand_vec3() * 2.0f;

	on_player_death();
}

void update_player_death(GLFWwindow *window, double dt)
{
	for (auto &part : player.parts)
	{
		part.pos += part.vel * float(dt);
		part.vel -= 0.2f * dt;
	}
}

void update_player(GLFWwindow *window, double dt)
{
	// Number of steps that correspond to a single player sphere
	static const int n = int(2.0f * player.radius / (player.speed * float(dt))) + 1;
	player.past_pos.push_back(player.pos);
	if (player.past_pos.size() == n)
		player.past_pos.erase(player.past_pos.begin());

	level_advect(player.pos, player.vel, float(dt));
	player.parts[0].pos = player.pos;
	player.parts[0].vel = player.vel;
	player.parts[0].past_pos = player.past_pos;

	for (int i = player.parts.size() - 1; i > 0; --i)
	{
		PlayerPart &p = player.parts[i];
		p.past_pos.push_back(p.pos);
		if (p.past_pos.size() == n)
			p.past_pos.erase(p.past_pos.begin());
		p.pos = player.parts[i - 1].past_pos[0];

		if (is_close(p.pos, player.pos, player.radius * 0.8f, player.radius * 0.8f))
			player_die();
	}

	ObjectType type;
	int id;
	if (level_collide_object(player.pos, player.radius, type, id))
	{
		switch (type)
		{
		case ObjectType::APPLE: append_part(); on_player_pickup(); break;
		case ObjectType::ENEMY: player_die(); break;
		}
		level_remove_object(id);
	}
}

void render_player(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	for (auto &part : player.parts) 
	{
		mat4 transform = translate(part.pos);
		shader->set_uniform("model", transform * scale(player.radius));
		render_colored(GL_TRIANGLES, *shader, *part.mesh);
	}
}

vec3 player_get_world_pos()
{
	return player.pos;
}

vec3 player_get_world_vel()
{
	return player.vel;
}

int player_get_length()
{
	return player.parts.size();
}

bool player_is_dead()
{
	return player.dead;
}

float player_get_radius()
{
	return player.radius;
}