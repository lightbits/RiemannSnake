#ifndef LEVEL_H
#define LEVEL_H
#include "util.h"
#include "types.h"
#include <vector>

enum class LevelTile { BLANK = 0, OBSTACLE = 1, CHERRY = 2 };

bool load_level(GLFWwindow *window);
void init_level(GLFWwindow *window, 
				int level_size, 
				float grid_scale, 
				const vec3 &grid_color);

void free_level(GLFWwindow *window);
void update_level(GLFWwindow *window, double dt);
void render_level(GLFWwindow *window, double dt);

LevelTile level_get_tile(int x, int y);
void level_clear_tiles(LevelTile tile);
void level_set_tile(int x, int y, LevelTile tile);
int level_get_size();
float level_get_cell_size();
vec2 level_to_world_pos(const vec2i &p);

#endif