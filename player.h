#ifndef PLAYER_H
#define PLAYER_H
#include "util.h"

bool load_player(GLFWwindow *window);
void init_player(GLFWwindow *window,
				 float player_speed,
				 float player_radius,
				 const vec3 &start_pos,
				 const vec4 &head_color, 
				 const vec4 &body_color);
void free_player(GLFWwindow *window);
void handle_player_input(GLFWwindow *window, double dt);
void update_player_death(GLFWwindow *window, double dt);
void update_player(GLFWwindow *window, double dt);
void render_player(GLFWwindow *window, double dt);

vec3 player_get_world_pos();
vec3 player_get_world_vel();
float player_get_radius();
int player_get_length();
bool player_is_dead();

#endif