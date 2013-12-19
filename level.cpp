#include "level.h"
#include "cube.h"

struct Level
{
	int size;
	float grid_scale;
	std::vector<LevelTile> tiles;
	Mesh mesh_grid;
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
				const vec3 &grid_color)
{
	level.size = level_size;
	level.grid_scale = grid_scale;
	level.tiles = std::vector<LevelTile>(level_size * level_size);
	level.mesh_grid = generate_grid(level_size, grid_scale, grid_color);

	level_clear_tiles(LevelTile::BLANK);
}

void free_level(GLFWwindow *window)
{
	level.tiles.clear();
	level.size = 0;
	delete_mesh(level.mesh_grid);
}

void update_level(GLFWwindow *window, double dt)
{
	
}

void render_level(GLFWwindow *window, double dt)
{
	Shader *shader = get_active_shader();
	shader->set_uniform("model", mat4(1.0f));
	render_pos_col(GL_LINES, *shader, level.mesh_grid);
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

vec2 level_to_world_pos(const vec2i &p)
{
	float cs = level_get_cell_size();
	return vec2(
		p.x * cs + cs / 2.0f - level.grid_scale / 2.0f,
		p.y * cs + cs / 2.0f - level.grid_scale / 2.0f);
}