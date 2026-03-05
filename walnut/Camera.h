#pragma once
#pragma once

const int RENDER_WIDTH = 640;
const int RENDER_HEIGHT = 360;

class Camera {
public:
    float x, y;

    Camera() : x(0), y(0) {}

    void update(float playerX, float playerY, int playerW, int playerH) {
        // center on player
        x = playerX + playerW / 2.0f - RENDER_WIDTH / 2.0f;
        y = playerY + playerH / 2.0f - RENDER_HEIGHT / 2.0f;

        // clamp to map bounds
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > MAP_COLUMNS * TILE_SIZE - RENDER_WIDTH)  x = (float)(MAP_COLUMNS * TILE_SIZE - RENDER_WIDTH);
        if (y > MAP_ROWS * TILE_SIZE - RENDER_HEIGHT) y = (float)(MAP_ROWS * TILE_SIZE - RENDER_HEIGHT);
    }
};