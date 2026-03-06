#include "Player.h"

const float GRAVITY = 800.0f;
const float MOVE_SPEED = 180.0f;
const float JUMP_FORCE = -380.0f;
const float DASH_SPEED = 500.0f;
const float DASH_TIME = 0.25f;
const float DASH_CD = 0.8f;
const float IFRAMES = 1.5f;  // seconds of invincibility after hit
//const float GROUND_Y = 500.0F; -- was fake ground

Player::Player() {
    x = 50; y = 16;
    velX = 0; velY = 0;
    normalHeight = 48;
    dashHeight = 24;
    width = 24;
    height = normalHeight;
    isDashing = false;
    dashTimer = 0.0f;
    dashCooldown = 0.0f;
    facingDir = 1;
    health = 4;
    iFrames = 0.0f;
}

int Player::getHitboxY() const {
    return (int)y;
}

int Player::getHitboxHeight() const {
    return height;
}

void Player::takeDamage(int amount) {
    if (iFrames > 0.0f) return; // ignore damage while invincible
    health -= amount;
    if (health < 0) health = 0;
    iFrames = IFRAMES;
}

bool Player::isAlive() const {
    return health > 0;
}

bool Player::isInvincible() const {
    return iFrames > 0.0f;
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
        velX = DASH_SPEED * facingDir;
    }
}

void Player::update(float dt, TileMap& tileMap) {
    // tick timers
    if (dashCooldown > 0.0f) dashCooldown -= dt;
    if (iFrames > 0.0f) iFrames -= dt;
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0.0f) {
            isDashing = false;
            height = normalHeight;
        }
    }

    velY += GRAVITY * dt;

    // simple collision with the ground
    x += velX * dt;
    int left = (int)x / TILE_SIZE;
    int right = ((int)x + width - 1) / TILE_SIZE;
    int top = getHitboxY() / TILE_SIZE;
    int bottom = (getHitboxY() + getHitboxHeight() - 1) / TILE_SIZE;

    for (int r = top; r <= bottom; r++) {
        if (velX > 0) {
            if (tileMap.isBreakable(r, right)) {
                if (isDashing) {
                    tileMap.breakTile(r, right); // break it
                }
                else {
                    x = (float)(right * TILE_SIZE - width); // treat as wall
                    velX = 0;
                }
            }
            else if (tileMap.map[r][right] == 1) {
                x = (float)(right * TILE_SIZE - width);
                velX = 0;
            }
        }
        if (velX < 0) {
            if (tileMap.isBreakable(r, left)) {
                if (isDashing) {
                    tileMap.breakTile(r, left); // break it
                }
                else {
                    x = (float)((left + 1) * TILE_SIZE); // treat as wall
                    velX = 0;
                }
            }
            else if (tileMap.map[r][left] == 1) {
                x = (float)((left + 1) * TILE_SIZE);
                velX = 0;
            }
        }
    }

    // air collision
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
    // flash while invincible
    if (iFrames > 0.0f && (int)(iFrames * 10) % 2 == 0) return;

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

void Player::renderHUD(SDL_Renderer* renderer) {
    int squareSize = 16;
    int padding = 4;
    int startX = 8;
    int startY = 8;

    for (int i = 0; i < 2; i++) { // 2 full heart squares
        int fullHalves = health - (i * 2); // how many halves remain for this square

        SDL_Rect outline = { startX + i * (squareSize + padding), startY, squareSize, squareSize };

        if (fullHalves >= 2) {
            // full heart — solid red
            SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
            SDL_RenderFillRect(renderer, &outline);
        }
        else if (fullHalves == 1) {
            // half heart — fill left half only
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &outline);
            SDL_Rect half = { outline.x, outline.y, squareSize / 2, squareSize };
            SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
            SDL_RenderFillRect(renderer, &half);
        }
        else {
            // empty heart — dark grey
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderFillRect(renderer, &outline);
        }

        // white outline
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &outline);
    }
}