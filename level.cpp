#include "level.h"
#include "cube.h"
#include <iostream>

struct Object
{
	ObjectType type;
	float rho, phi, theta;
	Mesh mesh;
};

struct Level
{
	float radius;
	vec4 edge_color;
	vec4 fill_color;
	Mesh mesh_sphere_wire;
	Mesh mesh_sphere_fill;

	std::vector<Object> objects;
};

Level level;

bool load_level(GLFWwindow *window)
{
	// Nothing to do here yet!
	return true;	
}

void init_level(GLFWwindow *window, 
				float radius,
				const vec4 &edge_color,
				const vec4 &fill_color)
{
	level.radius = radius;
	level.mesh_sphere_wire = generate_wireframe_sphere(radius, 16, 16, fill_color);
	level.mesh_sphere_fill = generate_sphere(radius, 16, 16, fill_color);
}

void free_level(GLFWwindow *window)
{
	delete_mesh(level.mesh_sphere_fill);
	delete_mesh(level.mesh_sphere_wire);

	for (auto &obj : level.objects)
		delete_mesh(obj.mesh);
}

void update_level(GLFWwindow *window, double dt)
{
	
}

void render_level(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	shader->set_uniform("model", mat4(1.0f));
	render_wireframe(GL_TRIANGLES, *shader, level.mesh_sphere_wire);
}

//vec3 level_to_world_pos(const vec3 &sp)
//{
//	float r = sp.x * sin(sp.y);
//	return vec3(
//		r * cos(sp.z), 
//		sp.x * cos(sp.y), 
//		r * sin(sp.z));
//}

vec3 level_get_normal(const vec3 &p)
{
	return glm::normalize(p);
}