#include "TileMap.h"

TileMap::TileMap() {
    int layout[MAP_ROWS][MAP_COLUMNS] = {
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,1 },
        { 0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0 },
        { 0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0 },
        { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0 },
        { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
    };
    memcpy(map, layout, sizeof(map));
}

void TileMap::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLUMNS; c++) {
            if (map[r][c] == 1) {
                SDL_Rect rect = { c * TILE_SIZE, r * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

bool TileMap::isSolid(int row, int col) {
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLUMNS) return false;
    return map[row][col] == 1;
}