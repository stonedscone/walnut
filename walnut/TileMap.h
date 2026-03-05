#pragma once
#include <SDL2/SDL.h>

const int TILE_SIZE = 32;
const int MAP_ROWS = 12;
const int MAP_COLUMNS = 20;

class TileMap {
public:
    int map[MAP_ROWS][MAP_COLUMNS];

    TileMap();
    void loadFromFile(const char* filename);
    void render(SDL_Renderer* renderer, float camX, float camY);
    bool isSolid(int row, int col);
};