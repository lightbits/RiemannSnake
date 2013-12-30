#include "level.h"
#include "cube.h"
#include "transform.h"
#include "noise.h"
#include <iostream>
#include <unordered_map>

const vec4 APPLE_COLOR = to_rgb(0xAEEE00FF);
const vec4 ENEMY_COLOR = to_rgb(0xA61D1DFF);

struct LevelObject
{
	ObjectType type;
	vec3 pos;
	Mesh *mesh;
};

struct Level
{
	float level_radius;
	float object_radius;
	vec4 edge_color;
	vec4 fill_color;
	int next_object_id;
	double spawn_interval;
	double spawn_timer;

	std::unordered_map<int, LevelObject> objects;
};

Level level;
Mesh mesh_level_sphere;
Mesh mesh_apple;
Mesh mesh_enemy;

bool load_level(GLFWwindow *window)
{
	// Nothing to do here yet!
	return true;	
}

void init_level(GLFWwindow *window, 
				float level_radius,
				float object_radius,
				double spawn_interval,
				const vec4 &edge_color,
				const vec4 &fill_color)
{
	level.level_radius = level_radius;
	level.object_radius = object_radius;
	level.edge_color = edge_color;
	level.fill_color = fill_color;
	level.next_object_id = 1;
	level.spawn_interval = spawn_interval;
	level.spawn_timer = spawn_interval;

	mesh_level_sphere = generate_wireframe_sphere(1.0f, 16, 16, edge_color);
	mesh_apple = generate_wireframe_sphere(1.0f, 8, 8, APPLE_COLOR);
	mesh_enemy = generate_wireframe_sphere(1.0f, 8, 8, ENEMY_COLOR);
}

void free_level(GLFWwindow *window)
{
	delete_mesh(mesh_level_sphere);
	delete_mesh(mesh_apple);
	delete_mesh(mesh_enemy);
	level.objects.clear();
}

void spawn_object(ObjectType type, const vec3 &pos)
{
	LevelObject obj;
	obj.type = type;
	obj.pos = pos;
	switch (type)
	{
	case ObjectType::APPLE: obj.mesh = &mesh_apple; break;
	case ObjectType::ENEMY: obj.mesh = &mesh_enemy; break;
	}
	level.objects[level.next_object_id++] = obj;
}

void update_level(GLFWwindow *window, double dt)
{
	level.spawn_timer -= dt;
	if (level.spawn_timer <= 0.0)
	{
		float x = frand() * 2.0f - 1.0f;
		float y = frand() * 2.0f - 1.0f;
		float z = frand() * 2.0f - 1.0f;
		vec3 pos(x, y, z);
		pos = glm::normalize(pos) * level_get_radius();

		if (frand() < 0.5f) spawn_object(ObjectType::ENEMY, pos);
		else spawn_object(ObjectType::APPLE, pos);
		level.spawn_timer += level.spawn_interval;
	}
}

void render_level(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	shader->set_uniform("model", scale(level.level_radius));
	render_wireframe(GL_TRIANGLES, *shader, mesh_level_sphere);

	for (auto &obj : level.objects)
	{
		shader->set_uniform("model", translate(obj.second.pos) * scale(level.object_radius));
		render_wireframe(GL_TRIANGLES, *shader, *obj.second.mesh);
	}
}

void level_advect(vec3 &p, vec3 &v, float dt)
{
	float speed_sqrd = glm::dot(v, v);
	float a = speed_sqrd / level_get_radius();
	v -= a * dt * level_get_normal(p);
	p += v * dt;
}

vec3 level_get_normal(const vec3 &pos)
{
	return glm::normalize(pos);
}

float level_get_radius()
{
	return level.level_radius;
}

void level_remove_object(int id)
{
	level.objects.erase(id);
}

bool level_collide_object(const vec3 &pos, float radius, ObjectType &type, int &id)
{
	for (auto &obj : level.objects)
	{
		if (is_close(obj.second.pos, pos, level.object_radius * 0.8f, radius))
		{
			type = obj.second.type;
			id = obj.first;
			return true;
		}
	}
	return false;
}