#include "level.h"
#include "cube.h"
#include <iostream>

struct Level
{
	int size;
	float grid_scale;
	std::vector<LevelTile> tiles;
	Mesh mesh_sphere;
};

Level level;

bool load_level(GLFWwindow *window)
{
	// Nothing to do here yet!
	return true;	
}

void init_level(GLFWwindow *window, 
				int level_size, 
				float grid_scale, 
				const vec4 &grid_color)
{
	level.size = level_size;
	level.grid_scale = grid_scale;
	level.tiles = std::vector<LevelTile>(level_size * level_size);
	level.mesh_sphere = generate_wireframe_sphere(1.0f, 16, 16, grid_color);

	level_clear_tiles(LevelTile::BLANK);

	level_set_tile(5, 5, LevelTile::APPLE);
}

void free_level(GLFWwindow *window)
{
	level.tiles.clear();
	level.size = 0;
	delete_mesh(level.mesh_sphere);
}

void update_level(GLFWwindow *window, double dt)
{
	
}

void render_level(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	shader->set_uniform("model", mat4(1.0f));
	
	glBindBuffer(GL_ARRAY_BUFFER, level.mesh_sphere.vbo);
	shader->set_attribfv("position", 3, 10, 0);
	shader->set_attribfv("barycentric", 3, 10, 3);
	shader->set_attribfv("color", 4, 10, 6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, level.mesh_sphere.ibo);
	glDrawElements(GL_TRIANGLES, level.mesh_sphere.index_count, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader->unset_attrib("position");
	shader->unset_attrib("color");
	shader->unset_attrib("barycentric");
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

LevelTile level_get_tile(int x, int y)
{
	return level.tiles[y * level.size + x];
}

void level_clear_tiles(LevelTile tile)
{
	for (int i = 0; i < level.size * level.size; ++i)
		level.tiles[i] = tile;
}

void level_set_tile(int x, int y, LevelTile tile)
{
	level.tiles[y * level.size + x] = tile;
}

int level_get_size()
{
	return level.size;
}

float level_get_cell_size()
{
	return level.grid_scale / float(level.size);
}

vec2 level_to_world_pos(const vec2 &p)
{
	float cs = level_get_cell_size();
	return vec2(
		p.x * cs + cs / 2.0f - level.grid_scale / 2.0f,
		p.y * cs + cs / 2.0f - level.grid_scale / 2.0f);
}