#ifndef LEVEL_H
#define LEVEL_H
#include "util.h"
#include "types.h"
#include <vector>

enum class ObjectType { Apple, Bomb };

bool load_level(GLFWwindow *window);
void init_level(GLFWwindow *window,
				float radius,
				const vec4 &edge_color,
				const vec4 &fill_color);

void free_level(GLFWwindow *window);
void update_level(GLFWwindow *window, double dt);
void render_level(GLFWwindow *window, double dt);

vec3 level_get_normal(const vec3 &p);

#endif