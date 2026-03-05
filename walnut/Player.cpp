#include "Player.h"

const float GRAVITY = 1500.0f;
const float MOVE_SPEED = 300.0f;
const float JUMP_FORCE = -600.0f;
const float GROUND_Y = 500.0F; // TEMPORARY FAKE GROUND

Player::Player() {
	x = 100; y = 300;
	velX = 0; velY = 0;
	width = 32; height = 48;
}

void Player::handleInput(const Uint8* keys, float dt) {
	velX = 0;
	if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) velX = -MOVE_SPEED;
	if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) velX = MOVE_SPEED;
	if ((keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_W]) && y + height >= GROUND_Y)
		velY = JUMP_FORCE;
}

void Player::update(float dt) {
	velY += GRAVITY * dt;
	x += velX * dt;
	y += velY * dt;

	//temporary ground
	if (y + height >= GROUND_Y) {
		y = GROUND_Y - height;
		velY = 0;
	}
}

void Player::render(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red rectangle
	SDL_Rect rect = {(int)x, (int)y, width, height};
	SDL_RenderFillRect(renderer, &rect);
}