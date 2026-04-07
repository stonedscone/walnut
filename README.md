# Walnut

> A 2D platformer built from scratch in C++ with SDL2. No engine. No shortcuts.

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange)
![Library](https://img.shields.io/badge/library-SDL2-green)
![Status](https://img.shields.io/badge/status-In%20Development-yellow)

---

## Overview

Walnut is a handcrafted 2D platformer written entirely in C++ using SDL2. The goal is a complete 16-bit-styled game with original pixel art, original music, original engine. Want it built without relying on any game framework or engine. Targeting a Steam release.

---

## Features (Current)

### Player
- Smooth movement with **delta-time physics** — framerate independent
- **Gravity and jumping** with tuned feel
- **Roll-dash** with directional input, cooldown timer, and input locking during dash
- Facing direction tracked for directional attacks and animation
- **4 hit-point health system** displayed as two half-heart squares in the HUD
- **Invincibility frames** after taking damage to prevent combo deaths

### World
- **TileMap system** — levels loaded from `.txt` files, no recompiling to edit levels
- **Solid tile collision** with AABB resolution on all four sides
- **Breakable tiles** — destroyed on dash contact, solid otherwise
- **Internal render resolution** scaled up to the window — pixel-perfect at any window size

### Camera
- Smooth camera that follows the player through the world

### Enemies
- **Enemy base class** with virtual `update()` and `render()`
- First enemy type: **walker** — patrols back and forth, damages player on contact

### Game States
- State machine managing gameplay, game over, and transitions

### HUD
- Two health squares drawn in screen space (unaffected by camera)
- Full, half, and empty heart states

---

## Architecture

```
walnut/
├── main.cpp          — window, renderer, render target, game loop
├── Player.h/.cpp     — input, physics, dash, health, collision
├── TileMap.h/.cpp    — map loading, tile collision, breakable tiles
├── Camera.h/.cpp     — world-to-screen offset tracking
├── Enemy.h/.cpp      — base enemy class
├── level1.txt        — level data (tile grid)
└── ...
```

**Rendering pipeline:**
1. All game objects draw to a fixed-resolution `SDL_Texture` (internal resolution)
2. That texture is stretched to fill the window — crisp pixel scaling at any size
3. HUD is drawn in screen space after the world pass

---

## Tech Stack

| Tool | Role |
|---|---|
| C++ | Core language |
| SDL2 | Window, renderer, input, timing |
| SDL_image | PNG texture loading *(planned)* |
| Aseprite | Pixel art and spritesheets *(in progress)* |
| Visual Studio | IDE (MSVC compiler) |
| vcpkg | Dependency management |

---

## Roadmap

- [ ] Sprite art — player (idle, run, jump, fall, dash), tiles, enemies
- [ ] Animation system — spritesheet cycling via SDL_image
- [ ] Additional enemy types
- [ ] Boss fights (5 planned)
- [ ] Item pickups and currency system
- [ ] Level 2–5 design
- [ ] Full HUD overhaul
- [ ] Main menu and pause screen
- [ ] Audio — SDL_mixer integration
- [ ] Steam release via Steamworks SDK

---

## Building

**Requirements:**
- Visual Studio 2022
- vcpkg with SDL2 installed: `vcpkg install sdl2:x64-windows`
- vcpkg integrated: `vcpkg integrate install`
- Build target set to **x64**

Clone the repo, open the `.sln`, set configuration to `x64 Debug` or `x64 Release`, and build.

---

## License

All code and assets are original work. Art, engine, and design by Samuel J. Feliciano.
