#ifndef PLAYER_H
#define PLAYER_H
#include "util.h"
#include "level.h"

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

vec3 player_get_pos();
vec3 player_get_vel();
int player_get_length();

#endif