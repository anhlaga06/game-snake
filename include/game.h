#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <snake.h>

#define MAX_HEIGHT  15
#define MAX_WIDTH   30

typedef struct {
    int x;
    int y;
} Food;

typedef enum {
    DIR_NONE,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef enum {
    STATE_MENU,
    STATE_GAME_RUNNING,
    STATE_GAME_PAUSE
} GameState;

typedef enum {
    MENU_1,
    MENU_2
} MenuType;

typedef struct {
    MenuType type;
    unsigned int active;
} Menu;

typedef struct {
    Node *snake;
    Food food;
    Direction direction;
    bool isExit;
    GameState state;
    Menu menu;
    unsigned int score;
} GameContext;

void gameInit(GameContext *context);
void gameRun(GameContext *context);
void gameExit();

void gameDeinit(GameContext *context);
void newGameInit(GameContext *context);

void gameUpdate(GameContext *context);

#endif