#include "TileMap.h"
#include <fstream>
#include <string>

TileMap::TileMap() {
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLUMNS; c++)
            map[r][c] = 0;
}

void TileMap::loadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename);
        return;
    }

    std::string line;
    int row = 0;
    while (std::getline(file, line) && row < MAP_ROWS) {
        for (int column = 0; column < (int)line.size() && column < MAP_COLUMNS; column++) {
			map[row][column] = line[column] - '0'; // convert char to int
        }
        row++;
    }
}   

void TileMap::render(SDL_Renderer* renderer, float camX, float camY) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLUMNS; c++) {
            if (map[r][c] == 1) {
                SDL_Rect rect = {
                    c * TILE_SIZE - (int)camX,
                    r * TILE_SIZE - (int)camY,
                    TILE_SIZE, TILE_SIZE
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

bool TileMap::isSolid(int row, int col) {
    if (row < 0 || row >= MAP_ROWS || col < 0 || col >= MAP_COLUMNS) return false;
    return map[row][col] == 1;
}