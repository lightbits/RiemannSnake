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

// Converts a spherical coordinate (rho, phi, theta), to
// a world-coordinate system (x, y, z) with the transformation
// x = rho * sin(phi) * cos(theta)
// y = rho * cos(phi)
// z = rho * sin(phi) * sin(theta)
vec3 level_to_world_pos(float rho, float phi, float theta);

#endif