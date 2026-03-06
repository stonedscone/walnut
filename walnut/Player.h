#pragma once
#include <SDL2/SDL.h>
#include "TileMap.h"

class Player {
public:
    float x, y;
    float velX, velY;
    int width, height;

    //dash
    bool isDashing;
    float dashTimer;
    float dashCooldown;
    int normalHeight;
    int dashHeight;
    int facingDir; // 1 = right, -1 = left

    Player();
    void handleInput(const Uint8* keys, float dt);
    void update(float dt, TileMap& tileMap);
    void render(SDL_Renderer* renderer, float camX, float camY);

    int getHitboxY() const;
    int getHitboxHeight() const;
};