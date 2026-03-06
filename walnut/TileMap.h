#pragma once
#include <SDL2/SDL.h>

const int TILE_SIZE = 16;
const int MAP_ROWS = 28;
const int MAP_COLUMNS = 165;

class TileMap {
public:
    int map[MAP_ROWS][MAP_COLUMNS];

    TileMap();
    void loadFromFile(const char* filename);
    void render(SDL_Renderer* renderer, float camX, float camY);
    bool isSolid(int row, int column);
    bool isBreakable(int row, int column);
    void breakTile(int row, int column);
};