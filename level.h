#ifndef LEVEL_H
#define LEVEL_H
#include "util.h"
#include "types.h"
#include <vector>

enum class ObjectType { APPLE, ENEMY, NONE };

bool load_level(GLFWwindow *window);
void init_level(GLFWwindow *window,
				float level_radius,
				float object_radius,
				double spawn_interval,
				const vec4 &edge_color,
				const vec4 &fill_color);

void free_level(GLFWwindow *window);
void update_level(GLFWwindow *window, double dt);
void render_level(GLFWwindow *window, double dt);

//////////////////////
// Helper functions //
//////////////////////

bool level_collide_object(const vec3 &pos, float radius, ObjectType &type, int &id);
void level_remove_object(int id);

vec3 level_get_normal(const vec3 &pos);

// Transports a particle at p with velocity v across the level surface,
// with resulting position p and velocity v.
void level_advect(vec3 &p, vec3 &v, float dt);
float level_get_radius();

#endif