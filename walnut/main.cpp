#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "TileMap.h"
#include "Camera.h"
#include "GameState.h"

// helper to draw centered text
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    int w = surface->w, h = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect dst = { (RENDER_WIDTH - w) / 2, y, w, h };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_DestroyTexture(texture);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "flaire", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* renderTarget = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        RENDER_WIDTH, RENDER_HEIGHT
    );

    TTF_Font* fontLarge = TTF_OpenFont("font.ttf", 24);
    TTF_Font* fontSmall = TTF_OpenFont("font.ttf", 12);

    Player player;
    TileMap tileMap;
    tileMap.loadFromFile("level1.txt");
    Camera camera;

    GameState state = STATE_TITLE;
    int titleSelection = 0; // 0 = Play, 1 = Exit

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0,   255 };

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    while (running) {
        // delta time
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f;
        lastTime = now;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN) {
                if (state == STATE_TITLE) {
                    if (event.key.keysym.sym == SDLK_UP)
                        titleSelection = 0;
                    if (event.key.keysym.sym == SDLK_DOWN)
                        titleSelection = 1;
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (titleSelection == 0) {
                            // reset and play
                            player = Player();
                            tileMap.loadFromFile("level1.txt");
                            state = STATE_PLAYING;
                        }
                        if (titleSelection == 1) running = false;
                    }
                }

                if (state == STATE_GAMEOVER) {
                    if (event.key.keysym.sym == SDLK_UP)
                        titleSelection = 0;
                    if (event.key.keysym.sym == SDLK_DOWN)
                        titleSelection = 1;
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (titleSelection == 0) {
                            // retry
                            player = Player();
                            tileMap.loadFromFile("level1.txt");
                            state = STATE_PLAYING;
                        }
                        if (titleSelection == 1) {
                            // exit to menu
                            player = Player();
                            state = STATE_TITLE;
                            titleSelection = 0;
                        }
                    }
                }
            }
        }

        // update
        if (state == STATE_PLAYING) {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            player.handleInput(keys, dt);
            player.update(dt, tileMap);
            camera.update(player.x, player.y, player.width, player.height);

            // death — fell off bottom of map
            if (player.y > MAP_ROWS * TILE_SIZE) {
                state = STATE_GAMEOVER;
                titleSelection = 0;
            }
        }

        // draw to render target
        SDL_SetRenderTarget(renderer, renderTarget);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (state == STATE_TITLE) {
            drawText(renderer, fontLarge, "FLAIRE", 100, white);
            drawText(renderer, fontSmall, "> PLAY", 200, titleSelection == 0 ? yellow : white);
            drawText(renderer, fontSmall, "> EXIT", 220, titleSelection == 1 ? yellow : white);
        }

        if (state == STATE_PLAYING) {
            tileMap.render(renderer, camera.x, camera.y);
            player.render(renderer, camera.x, camera.y);
        }

        if (state == STATE_GAMEOVER) {
            // still draw the world behind the overlay
            tileMap.render(renderer, camera.x, camera.y);
            player.render(renderer, camera.x, camera.y);

            // overlay
            drawText(renderer, fontLarge, "GAME OVER", 120, white);
            drawText(renderer, fontSmall, "> RETRY", 200, titleSelection == 0 ? yellow : white);
            drawText(renderer, fontSmall, "> EXIT TO MENU", 220, titleSelection == 1 ? yellow : white);
        }

        // stretch to window
        SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, renderTarget, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(fontLarge);
    TTF_CloseFont(fontSmall);
    TTF_Quit();
    SDL_DestroyTexture(renderTarget);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}