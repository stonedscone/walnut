#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Player.h"
#include "TileMap.h"
#include "Camera.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "flaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // fixed internal resolution — all game logic runs at this size
    SDL_Texture* renderTarget = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        RENDER_WIDTH, RENDER_HEIGHT
    );

    Player player;
    TileMap tileMap;
    tileMap.loadFromFile("level1.txt");
    Camera  camera;

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    while (running) {
        // delta time
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        lastTime = now;

        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) running = false;

        const Uint8* keys = SDL_GetKeyboardState(NULL);
        player.handleInput(keys, dt);
        player.update(dt, tileMap);
        camera.update(player.x, player.y, player.width, player.height);

        // draw everything to small texture
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        tileMap.render(renderer, camera.x, camera.y);
        player.render(renderer, camera.x, camera.y);

        // stretch small texture to fill window
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, renderTarget, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(renderTarget);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}