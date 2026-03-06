#pragma once
#include <SDL2/SDL.h>
#include "TileMap.h"

class Player {
public:
    float x, y;
    float velX, velY;
    int width, height;
    int normalHeight;
    int dashHeight;
    bool isDashing;
    float dashTimer;
    float dashCooldown;
    int facingDir;

    // health
    int health;        // max 4 half hearts
    float iFrames;     // invincibility timer after hit

    Player();
    void handleInput(const Uint8* keys, float dt);
    void update(float dt, TileMap& tileMap);
    void render(SDL_Renderer* renderer, float camX, float camY);
    void renderHUD(SDL_Renderer* renderer);

    int getHitboxY()      const;
    int getHitboxHeight() const;

    void takeDamage(int amount);
    bool isAlive() const;
    bool isInvincible() const;
};