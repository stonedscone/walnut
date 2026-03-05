#pragma once
#include <SDL2/SDL.h>
#include "TileMap.h"

class Player {
public:
    float x, y;
    float velX, velY;
    int width, height;

    Player();
    void handleInput(const Uint8* keys, float dt);
    void update(float dt, TileMap& tileMap);
    void render(SDL_Renderer* renderer, float camX, float camY);
};