#include "Player.h"

const float GRAVITY = 800.0f;
const float MOVE_SPEED = 180.0f;
const float JUMP_FORCE = -380.0f;
//const float GROUND_Y = 500.0F; -- was fake ground

Player::Player() {
    x = 100; y = 200;
    velX = 0; velY = 0;
    width = 20; height = 30;
}

void Player::handleInput(const Uint8* keys, float dt) {
    velX = 0;
    if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])  velX = -MOVE_SPEED;
    if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) velX = MOVE_SPEED;
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_W]) && velY == 0)
        velY = JUMP_FORCE;
}

void Player::update(float dt, TileMap& tileMap) {
    velY += GRAVITY * dt;

    // simple collision with ground
    x += velX * dt;
    int left = (int)x / TILE_SIZE;
    int right = ((int)x + width - 1) / TILE_SIZE;
    int top = (int)y / TILE_SIZE;
    int bottom = ((int)y + height - 1) / TILE_SIZE;

    for (int r = top; r <= bottom; r++) {
        if (velX > 0 && tileMap.isSolid(r, right)) {
            x = (float)(right * TILE_SIZE - width);
            velX = 0;
        }
        if (velX < 0 && tileMap.isSolid(r, left)) {
            x = (float)((left + 1) * TILE_SIZE);
            velX = 0;
        }
    }

    // air collision
    y += velY * dt;
    left = (int)x / TILE_SIZE;
    right = ((int)x + width - 1) / TILE_SIZE;
    top = (int)y / TILE_SIZE;
    bottom = ((int)y + height - 1) / TILE_SIZE;

    for (int c = left; c <= right; c++) {
        if (velY > 0 && tileMap.isSolid(bottom, c)) {
            y = (float)(bottom * TILE_SIZE - height);
            velY = 0;
        }
        if (velY < 0 && tileMap.isSolid(top, c)) {
            y = (float)((top + 1) * TILE_SIZE);
            velY = 0;
        }
    }

    /*temporary ground code
    if (y + height >= GROUND_Y) {
        y = GROUND_Y - height;
        velY = 0;
    }
    */
}

void Player::render(SDL_Renderer* renderer, float camX, float camY) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red rectangle
    SDL_Rect rect = { (int)x - (int)camX, (int)y - (int)camY, width, height };
    SDL_RenderFillRect(renderer, &rect);
}