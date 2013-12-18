#ifndef LEVEL_H
#define LEVEL_H
#include "types.h"
#include <vector>

enum WorldTiles { TileBlank = 0, TileObstacle = 1, TileCherry = 2 };

class Level
{
public:
	Level() : tiles(), size(0) { }

	void init(int level_size)
	{
		size = level_size;
		clear_tiles(TileBlank);
	}

	void clear_tiles(int type)
	{
		tiles = std::vector<int>(size * size);
		for (int i = 0; i < size * size; ++i)
			tiles[i] = type;
	}

	void set_tile(int x, int y, int type) { tiles[y * size + x] = type; }
	int get_tile(int x, int y) const { return tiles[y * size + x]; }

	int get_size() const { return size;  }
private:
	std::vector<int> tiles;
	int size;
};

#endif