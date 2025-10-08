# Snake (C++ & SDL2)

A clean, resume-ready C++ game project using **SDL2**. Controls with arrow keys; includes player snake, three A.I. enemy snakes, and multiple food items.

## Project Structure
```
snake-sdl2/
├─ src/
│  └─ main.cpp
├─ docs/
│  ├─ مستند پروژه بازی مار.md
│  ├─ مستند پروژه بازی مار.txt
│  └─ snake_game_doc.pdf
├─ images/
├─ build/                 # gitignored
├─ CMakeLists.txt
├─ Makefile
├─ Dockerfile
├─ LICENSE
└─ .gitignore
```

## Build & Run (Linux/macOS) — Makefile
```bash
# SDL2 must be installed first (Ubuntu/Debian: sudo apt-get install libsdl2-dev)
make
make run
```

## Build (Cross-platform) — CMake
```bash
cmake -S . -B build
cmake --build build
```

## Run in Docker
```bash
docker build -t snake-sdl2 .
docker run --rm -it snake-sdl2
```
> Headless containers won't show the window; prefer running locally for graphics.

## Roadmap
- [ ] Score HUD and game-over/win screen
- [ ] Enemy pathfinding (A*)
- [ ] Settings menu (speed, board size)
- [ ] GitHub Actions CI for Linux/Windows builds
- [ ] VS Code devcontainer