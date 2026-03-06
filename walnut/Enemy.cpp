#include "Enemy.h"
#include <cmath>

const float AGGRO_RANGE = 5 * 16.0f; // 5 tiles in pixels

Enemy::Enemy(float startX, float startY) {
    x = startX;
    y = startY;
    velX = 40.0f;
    velY = 0.0f;
    width = 24;
    height = 32;
    health = 2;
    active = true;
    patrolSpeed = 40.0f;
    aggroSpeed = 80.0f;
    isAggroed = false;
}

void Enemy::update(float dt, TileMap& tileMap, float playerX, float playerY) {
    if (!active) return;

    // check distance to player
    float dx = playerX - x;
    float dy = playerY - y;
    float dist = sqrtf(dx * dx + dy * dy);

    if (dist < AGGRO_RANGE) {
        isAggroed = true;
    }
    else {
        isAggroed = false;
    }

    if (isAggroed) {
        // move toward player horizontally
        float speed = aggroSpeed;
        velX = (dx > 0) ? speed : -speed;
    }

    velY += 800.0f * dt;

    // move X first, resolve X collision
    x += velX * dt;

    int left = (int)x / TILE_SIZE;
    int right = ((int)x + width - 1) / TILE_SIZE;
    int top = (int)y / TILE_SIZE;
    int bottom = ((int)y + height - 1) / TILE_SIZE;

    bool hitWall = false;
    for (int r = top; r <= bottom; r++) {
        if (velX > 0 && tileMap.isSolid(r, right) && !hitWall) {
            x = (float)(right * TILE_SIZE - width);
            if (!isAggroed) velX = -velX; // only flip if patrolling
            else velX = 0;
            hitWall = true;
        }
        if (velX < 0 && tileMap.isSolid(r, left) && !hitWall) {
            x = (float)((left + 1) * TILE_SIZE);
            if (!isAggroed) velX = -velX;
            else velX = 0;
            hitWall = true;
        }
    }

    // move Y, resolve Y collision
    y += velY * dt;

    left = (int)x / TILE_SIZE;
    right = ((int)x + width - 1) / TILE_SIZE;
    top = (int)y / TILE_SIZE;
    bottom = ((int)y + height - 1) / TILE_SIZE;

    for (int c = left; c <= right; c++) {
        if (velY > 0 && tileMap.isSolid(bottom, c)) {
            y = (float)(bottom * TILE_SIZE - height);
            velY = 0;
            // resume patrol if not aggroed
            if (!isAggroed && velX == 0) velX = patrolSpeed;
        }
        if (velY < 0 && tileMap.isSolid(top, c)) {
            y = (float)((top + 1) * TILE_SIZE);
            velY = 0;
        }
    }

    if (y > MAP_ROWS * TILE_SIZE) active = false;
}

void Enemy::render(SDL_Renderer* renderer, float camX, float camY) {
    if (!active) return;
    // turn red when aggroed
    if (isAggroed) {
        SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
    }
    SDL_Rect rect = { (int)x - (int)camX, (int)y - (int)camY, width, height };
    SDL_RenderFillRect(renderer, &rect);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health <= 0) active = false;
}

bool Enemy::isAlive() const {
    return active;
}

bool Enemy::overlaps(float ox, float oy, int ow, int oh) const {
    return x < ox + ow && x + width > ox &&
        y < oy + oh && y + height > oy;
}