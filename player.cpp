#include "player.h"

//void init_player()
//{
//	PlayerBlock block0;
//	block0.pos_x = 0;
//	block0.pos_y = 0;
//	block0.vel_x = 0;
//	block0.vel_y = 0;
//
//	player.blocks.push_back(block0);
//
//	player.pos_x = 0;
//	player.pos_y = 0;
//	player.vel_x = 0;
//	player.vel_y = 0;
//
//	cube = generate_color_cube(1.0f, 1.0f, 0.23f, 0.21f);
//}
//
//void set_player_velocity(int x, int y) 
//{
//	player.vel_x = x; player.vel_y = y; 
//}
//
//void set_player_position(int x, int y)
//{
//	if (x < 0) x = LEVEL_SIZE - 1;
//	if (x >= LEVEL_SIZE) x = 0;
//	if (y < 0) y = LEVEL_SIZE - 1;
//	if (y >= LEVEL_SIZE) y = 0;
//	player.pos_x = x;
//	player.pos_y = y;
//}
//
//void update_player(GLFWwindow *window, double dt)
//{
//	if (glfwGetKey(window, GLFW_KEY_LEFT))
//		set_player_velocity(-1, 0);
//	else if (glfwGetKey(window, GLFW_KEY_RIGHT))
//		set_player_velocity(1, 0);
//
//	if (glfwGetKey(window, GLFW_KEY_UP))
//		set_player_velocity(0, -1);
//	else if (glfwGetKey(window, GLFW_KEY_DOWN))
//		set_player_velocity(0, 1);
//}
//
//void update_player_position()
//{
//	set_player_position(player.pos_x + player.vel_x, player.pos_y + player.vel_y);
//}
//
//void render_player()
//{
//	vec2 world_pos = level_to_world(player.pos_x, player.pos_y);
//	mat4 transform = 
//		translate(world_pos.x, CELL_SIZE / 2.0f, world_pos.y) *
//		scale(CELL_SIZE);
//	default_shader.set_uniform("model", transform);
//	render_pos_col(GL_TRIANGLES, default_shader, cube);
//}
//
//void update_player(GLFWwindow *window, double dt, const Level &level)
//{
//
//}
//
//void render_player(GLFWwindow *window)
//{
//
//}