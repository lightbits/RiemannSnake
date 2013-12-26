#ifndef PLAYER_H
#define PLAYER_H
#include "util.h"
#include "level.h"

struct Spherical 
{ 
	Spherical() : rho(0.0f), phi(0.0f), theta(0.0f) { }
	Spherical(float Rho, float Phi, float Theta) : rho(Rho), phi(Phi), theta(Theta) { }
	float rho, phi, theta; 
};

bool load_player(GLFWwindow *window);
void init_player(GLFWwindow *window,
				 int start_length, 
				 float player_speed,
				 const vec3 &start_pos,
				 const vec4 &head_color, 
				 const vec4 &body_color);
void free_player(GLFWwindow *window);
void handle_player_input(GLFWwindow *window, double dt);
void update_player(GLFWwindow *window, double dt);
void render_player(GLFWwindow *window, double dt);

Spherical player_get_sphere_pos();
vec3 player_get_world_pos();
vec2 player_get_tan_vel();
vec3 player_get_world_vel();
int player_get_length();

#endif