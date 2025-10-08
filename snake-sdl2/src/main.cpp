#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const int CELL_SIZE = 10;

enum Direction { NONE, UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
};

class Snake {
public:
    deque<Point> body;
    Direction dir;
    int length;
    SDL_Color color;

    Snake(int startX, int startY, SDL_Color snakeColor) : color(snakeColor) {
        body.push_back({startX, startY});
        dir = NONE;
        length = 1;
    }

    void move() {
        if (dir == NONE) return;

        Point newHead = body.front();
        switch (dir) {
            case UP: newHead.y -= CELL_SIZE; break;
            case DOWN: newHead.y += CELL_SIZE; break;
            case LEFT: newHead.x -= CELL_SIZE; break;
            case RIGHT: newHead.x += CELL_SIZE; break;
        }
        body.push_front(newHead);
        if (body.size() > length) {
            body.pop_back();
        }
    }

    void grow() {
        length++;
    }

    bool checkCollision(Point p) {
        for (auto part : body) {
            if (part.x == p.x && part.y == p.y) {
                return true;
            }
        }
        return false;
    }

    bool checkSelfCollision() {
        for (size_t i = 1; i < body.size(); ++i) {
            if (body[i].x == body[0].x && body[i].y == body[0].y) {
                return true;
            }
        }
        return false;
    }

    bool isOutOfBounds() {
        Point head = body.front();
        return head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT;
    }

    void setDirection(Direction newDirection) {
        dir = newDirection;
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
        for (auto part : body) {
            SDL_Rect rect = { part.x, part.y, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    Point getHeadPosition() {
        return body.front();
    }

    int getLength() {
        return length;
    }
};

class Food {
public:
    Food(int screenWidth, int screenHeight)
        : screenWidth(screenWidth), screenHeight(screenHeight) {
        generate();
    }

    void generate() {
        position.x = (rand() % (screenWidth / CELL_SIZE)) * CELL_SIZE;
        position.y = (rand() % (screenHeight / CELL_SIZE)) * CELL_SIZE;
    }

    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect rect = { position.x, position.y, CELL_SIZE, CELL_SIZE };
        SDL_RenderFillRect(renderer, &rect);
    }

    Point getPosition() {
        return position;
    }

private:
    Point position;
    int screenWidth, screenHeight;
};

class Game {
public:
    Game()
        : player(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, {0, 255, 0, 255}), isRunning(true), gameWon(false) {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Snake Game",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        srand(time(0));

        // Add enemies with different colors
        enemies.push_back(Snake(100, 100, {255, 0, 0, 255}));
        enemies.push_back(Snake(200, 200, {0, 0, 255, 255}));
        enemies.push_back(Snake(300, 300, {255, 255, 0, 255}));

        // Generate multiple food items
        for (int i = 0; i < 3; ++i) {
            foods.push_back(Food(SCREEN_WIDTH, SCREEN_HEIGHT));
        }
    }

    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void run() {
        Uint32 enemyMoveTimer = SDL_GetTicks();
        while (isRunning) {
            handleEvents();
            update();
            render();
            checkCollisions();

            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - enemyMoveTimer > 500) { // سرعت بیشتر برای حرکت مارهای دشمن
                moveEnemies();
                enemyMoveTimer = currentTime;
            }

            SDL_Delay(100);
        }
    }

private:
    void handleEvents() {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                isRunning = false;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP: player.setDirection(UP); break;
                    case SDLK_DOWN: player.setDirection(DOWN); break;
                    case SDLK_LEFT: player.setDirection(LEFT); break;
                    case SDLK_RIGHT: player.setDirection(RIGHT); break;
                }
            } else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_UP && player.dir == UP) player.setDirection(NONE);
                if (e.key.keysym.sym == SDLK_DOWN && player.dir == DOWN) player.setDirection(NONE);
                if (e.key.keysym.sym == SDLK_LEFT && player.dir == LEFT) player.setDirection(NONE);
                if (e.key.keysym.sym == SDLK_RIGHT && player.dir == RIGHT) player.setDirection(NONE);
            }
        }
    }

    void update() {
        player.move();
        for (auto& food : foods) {
            if (player.checkCollision(food.getPosition())) {
                player.grow();
                food.generate();
            }
        }
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        player.render(renderer);
        for (auto& food : foods) {
            food.render(renderer);
        }

        for (auto& enemy : enemies) {
            enemy.render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    void checkCollisions() {
        if (player.isOutOfBounds() || player.checkSelfCollision()) {
            isRunning = false;
            gameWon = false;
        }

        for (auto it = enemies.begin(); it != enemies.end(); ) {
            if (player.checkCollision(it->getHeadPosition())) {
                if (player.getLength() > it->getLength()) {
                    it = enemies.erase(it);
                } else {
                    isRunning = false;
                    gameWon = false;
                    break;
                }
            } else if (it->isOutOfBounds() || it->checkSelfCollision()) {
                it = enemies.erase(it);
            } else {
                ++it;
            }
        }

        if (enemies.empty()) {
            isRunning = false;
            gameWon = true;
        }
    }

    void moveEnemies() {
        for (auto& enemy : enemies) {
            Direction dirs[4] = { UP, DOWN, LEFT, RIGHT };
            enemy.setDirection(dirs[rand() % 4]);
            enemy.move();
        }
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
    bool gameWon;
    Snake player;
    std::vector<Snake> enemies;
    std::vector<Food> foods;
};

int main(int argc, char* argv[]) {
    Game game;
    game.run();
    return 0;
}