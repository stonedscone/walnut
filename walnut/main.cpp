#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Player.h"
#include "TileMap.h"
#include "Camera.h"
#include "GameState.h"
#include "Enemy.h"
#include <vector>

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

    if (!fontLarge || !fontSmall) {
        SDL_Log("Font failed: %s", TTF_GetError());
        return 1;
    }

    Player player;
    TileMap tileMap;
    tileMap.loadFromFile("level1.txt");
    Camera camera;

    // spawn enemies across all floors
    std::vector<Enemy> enemies;
    enemies.push_back(Enemy(100, 16));
    enemies.push_back(Enemy(600, 16));
    enemies.push_back(Enemy(200, 96));
    enemies.push_back(Enemy(800, 96));
    enemies.push_back(Enemy(400, 176));
    enemies.push_back(Enemy(1000, 176));
    enemies.push_back(Enemy(300, 256));
    enemies.push_back(Enemy(900, 256));
    enemies.push_back(Enemy(1500, 256));

    GameState state = STATE_TITLE;
    int titleSelection = 0;

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0,   255 };

    Uint32 lastTime = SDL_GetTicks();
    bool running = true;
    SDL_Event event;

    while (running) {
        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTime) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f;
        lastTime = now;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN) {
                if (state == STATE_TITLE) {
                    if (event.key.keysym.sym == SDLK_UP)   titleSelection = 0;
                    if (event.key.keysym.sym == SDLK_DOWN) titleSelection = 1;
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (titleSelection == 0) {
                            player = Player();
                            tileMap.loadFromFile("level1.txt");
                            enemies.clear();
                            enemies.push_back(Enemy(100, 16));
                            enemies.push_back(Enemy(600, 16));
                            enemies.push_back(Enemy(200, 96));
                            enemies.push_back(Enemy(800, 96));
                            enemies.push_back(Enemy(400, 176));
                            enemies.push_back(Enemy(1000, 176));
                            enemies.push_back(Enemy(300, 256));
                            enemies.push_back(Enemy(900, 256));
                            enemies.push_back(Enemy(1500, 256));
                            state = STATE_PLAYING;
                        }
                        if (titleSelection == 1) running = false;
                    }
                }

                if (state == STATE_GAMEOVER) {
                    if (event.key.keysym.sym == SDLK_UP)   titleSelection = 0;
                    if (event.key.keysym.sym == SDLK_DOWN) titleSelection = 1;
                    if (event.key.keysym.sym == SDLK_RETURN) {
                        if (titleSelection == 0) {
                            player = Player();
                            tileMap.loadFromFile("level1.txt");
                            enemies.clear();
                            enemies.push_back(Enemy(100, 16));
                            enemies.push_back(Enemy(600, 16));
                            enemies.push_back(Enemy(200, 96));
                            enemies.push_back(Enemy(800, 96));
                            enemies.push_back(Enemy(400, 176));
                            enemies.push_back(Enemy(1000, 176));
                            enemies.push_back(Enemy(300, 256));
                            enemies.push_back(Enemy(900, 256));
                            enemies.push_back(Enemy(1500, 256));
                            state = STATE_PLAYING;
                        }
                        if (titleSelection == 1) {
                            player = Player();
                            state = STATE_TITLE;
                            titleSelection = 0;
                        }
                    }
                }
            }
        }

        if (state == STATE_PLAYING) {
            const Uint8* keys = SDL_GetKeyboardState(NULL);
            player.handleInput(keys, dt);
            player.update(dt, tileMap);
            camera.update(player.x, player.y, player.width, player.height);

            for (auto& e : enemies) {
                e.update(dt, tileMap, player.x, player.y);

                if (e.active && e.overlaps(player.x, player.y, player.width, player.height)) {
                    if (player.isDashing) {
                        e.takeDamage(2);
                    }
                    else {
                        player.takeDamage(1);
                    }
                }
            }

            // death
            if (player.y > MAP_ROWS * TILE_SIZE || !player.isAlive()) {
                state = STATE_GAMEOVER;
                titleSelection = 0;
            }
        }

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
            for (auto& e : enemies) e.render(renderer, camera.x, camera.y);
            player.render(renderer, camera.x, camera.y);
            player.renderHUD(renderer);
        }

        if (state == STATE_GAMEOVER) {
            tileMap.render(renderer, camera.x, camera.y);
            for (auto& e : enemies) e.render(renderer, camera.x, camera.y);
            player.render(renderer, camera.x, camera.y);
            player.renderHUD(renderer);

            // dark overlay
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
            SDL_Rect overlay = { 0, 0, RENDER_WIDTH, RENDER_HEIGHT };
            SDL_RenderFillRect(renderer, &overlay);
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

            drawText(renderer, fontLarge, "GAME OVER", 120, white);
            drawText(renderer, fontSmall, "> RETRY", 200, titleSelection == 0 ? yellow : white);
            drawText(renderer, fontSmall, "> EXIT TO MENU", 220, titleSelection == 1 ? yellow : white);
        }

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