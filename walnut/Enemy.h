#pragma once
#include <SDL2/SDL.h>
#include "TileMap.h"

class Enemy {
public:
    float x, y;
    float velX, velY;
    int width, height;
    int health;
    bool active;

    float patrolSpeed;  // normal walk speed
    float aggroSpeed;   // chase speed
    bool  isAggroed;    // currently chasing player

    Enemy(float startX, float startY);
    virtual void update(float dt, TileMap& tileMap, float playerX, float playerY);
    virtual void render(SDL_Renderer* renderer, float camX, float camY);
    void takeDamage(int amount);
    bool isAlive() const;
    bool overlaps(float ox, float oy, int ow, int oh) const;
};