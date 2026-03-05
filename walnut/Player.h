#pragma once
#include <SDL2/SDL.h>

class Player {
public:
	float x, y;
	float velX, velY;
	int width, height;

	Player();
	void handleInput(const Uint8* keys, float dt);
	void update(float dt);
	void render(SDL_Renderer* renderer);
};