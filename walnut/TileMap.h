#pragma once
#include <SDL2/SDL.h>

const int TILE_SIZE = 48;
const int MAP_ROWS = 13;
const int MAP_COLUMNS = 17;

class TileMap {
public:
	int map[MAP_ROWS][MAP_COLUMNS];

	TileMap();
	void render(SDL_Renderer* renderer);
	bool isSolid(int row, int col);
};