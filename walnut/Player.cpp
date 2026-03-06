#include "Player.h"

const float GRAVITY = 800.0f;
const float MOVE_SPEED = 180.0f;
const float JUMP_FORCE = -380.0f;
const float DASH_SPEED = 500.0f;
const float DASH_TIME = 0.25f;
const float DASH_CD = 0.8f;
//const float GROUND_Y = 500.0F; -- was fake ground

Player::Player() {
    x = 100; y = 200;
    velX = 0; velY = 0;
    normalHeight = 30;
    dashHeight = 15;
    width = 20;
    height = normalHeight; // always normal height
    isDashing = false;
    dashTimer = 0.0f;
    dashCooldown = 0.0f;
    facingDir = 1;
}

int Player::getHitboxY() const {
    return (int)y;
}

int Player::getHitboxHeight() const {
    return height;
}

void Player::handleInput(const Uint8* keys, float dt) {
    // don't accept movement input while dashing
    if (!isDashing) {
        velX = 0;
        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) { velX = -MOVE_SPEED; facingDir = -1; }
        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) { velX = MOVE_SPEED; facingDir = 1; }
    }

    // jump
    if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_W]) && velY == 0)
        velY = JUMP_FORCE;

    // dash
    if (keys[SDL_SCANCODE_LSHIFT] && dashCooldown <= 0.0f && !isDashing) {
        isDashing = true;
        dashTimer = DASH_TIME;
        dashCooldown = DASH_CD;
        // removed height change
        velX = DASH_SPEED * facingDir;
    }
}

void Player::update(float dt, TileMap& tileMap) {
    //tick timers
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0.0f) {
            isDashing = false;
            height = normalHeight;
        }
    }

    velY += GRAVITY * dt;

    //simple collision with the ground
    x += velX * dt;
    int left = (int)x / TILE_SIZE;
    int right = ((int)x + width - 1) / TILE_SIZE;
    int top = getHitboxY() / TILE_SIZE;
    int bottom = (getHitboxY() + getHitboxHeight() - 1) / TILE_SIZE;

for (int r = top; r <= bottom; r++) {
    if (velX > 0) {
        if (tileMap.isBreakable(r, right) && isDashing) {
            tileMap.breakTile(r, right);
        } else if (tileMap.map[r][right] == 1) {
            x = (float)(right * TILE_SIZE - width);
            velX = 0;
        }
    }
    if (velX < 0) {
        if (tileMap.isBreakable(r, left) && isDashing) {
            tileMap.breakTile(r, left);
        } else if (tileMap.map[r][left] == 1) {
            x = (float)((left + 1) * TILE_SIZE);
            velX = 0;
        }
    }
}

    //air collision
    y += velY * dt;
    left = (int)x / TILE_SIZE;
    right = ((int)x + width - 1) / TILE_SIZE;
    top = getHitboxY() / TILE_SIZE;
    bottom = (getHitboxY() + getHitboxHeight() - 1) / TILE_SIZE;

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
    // during dash draw only the lower hitbox portion in a lighter color
    if (isDashing) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); // lighter red while dashing
        SDL_Rect rect = { (int)x - (int)camX, getHitboxY() - (int)camY, width, dashHeight };
        SDL_RenderFillRect(renderer, &rect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red rectangle
        SDL_Rect rect = { (int)x - (int)camX, (int)y - (int)camY, width, height };
        SDL_RenderFillRect(renderer, &rect);
    }
}