#include "game.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "input.h"
#include "util_os.h"
#include <string.h>
#include "menu.h"


char screenBuffer[(MAX_HEIGHT + 1) * (MAX_WIDTH + 1) + 1];

void gameInit(GameContext *context) {
    printf("\033[2J");      // clear
    printf("\033[?25l");    // hide cursor
    
    memset(context, 0, sizeof(*context));
    
    context->state = STATE_MENU;
    context->menu.type = MENU_1;
    context->menu.active = 1;
    context->isExit = false;
}

void gameExit() {
    printf("\033[?25h");  // show cursor
}

int inputToDirection(char input) {
    if (input == 'a' || input == 75) return DIR_LEFT;
    if (input == 'd' || input == 77) return DIR_RIGHT;
    if (input == 's' || input == 80) return DIR_DOWN;
    if (input == 'w' || input == 72) return DIR_UP;
    return DIR_NONE;
}

int calcPosition(int x, int y) {
    return y * (MAX_WIDTH + 1) + x;
}

bool isOppositeDirection(int dir1, int dir2) {
    if (dir1 == DIR_LEFT && dir2 == DIR_RIGHT) return true;
    if (dir2 == DIR_LEFT && dir1 == DIR_RIGHT) return true;
    if (dir1 == DIR_UP && dir2 == DIR_DOWN) return true;
    if (dir2 == DIR_UP && dir1 == DIR_DOWN) return true;
    return false;
}

void handleInput(GameContext *context){
    char userInput;
    if (readInput(&userInput)) {
        int nextDirect = inputToDirection(userInput);
        switch (context->state)
        {
            case STATE_GAME_RUNNING: {
                if (nextDirect != DIR_NONE && !isOppositeDirection(context->direction, nextDirect)) {
                    context->direction = nextDirect;
                }
                break;
            }
            case STATE_GAME_PAUSE: {
                break;
            }
            case STATE_MENU: {
                handleMenuInput(context, userInput);
                break;
            }
            default:
                break;
        }
    }
}

void generateFood(GameContext *context) {
    while (true)
    {
        Food *foodPtr = &context->food;
        foodPtr->x = (rand() % (MAX_WIDTH - 2)) + 1;
        foodPtr->y = (rand() % (MAX_HEIGHT - 2)) + 1;
        bool isFoodOverlapSnake = false;
        Node *node = context->snake;
        while (node)
        {
            if (node->x == foodPtr->x && node->y == foodPtr->y) {
                isFoodOverlapSnake = true;
                break;
            }
            node = node->next;
        }
        if (!isFoodOverlapSnake) {
            break;
        }
    }
    
}
void drawBackground() {
    for (int heightIdx = 0; heightIdx < MAX_HEIGHT; heightIdx++) {
        for (int widthIdx = 0; widthIdx < MAX_WIDTH; widthIdx++) {
            char r = ' ';
            bool isEdge = (heightIdx == 0) || (heightIdx == MAX_HEIGHT - 1) 
            || (widthIdx == 0) || (widthIdx == MAX_WIDTH - 1);
            if (isEdge) r = '#';
            int charIdx = calcPosition(widthIdx, heightIdx);
            screenBuffer[charIdx] = r;
            bool isEndLine = (widthIdx == MAX_WIDTH - 1);
            if (isEndLine) screenBuffer[charIdx + 1] = '\n';
        }
    }
}
void drawSnake(Node *snake) {
    int headPos = calcPosition(snake->x, snake->y);
    screenBuffer[headPos] = 'O';
    Node *snakeNode = snake->next;
    while (snakeNode)
    {
        int bodyPos = calcPosition(snakeNode->x, snakeNode->y);
        screenBuffer[bodyPos] = 'o';
        snakeNode = snakeNode->next;
    }
}
void drawFood(Food food) {
    int pos = calcPosition(food.x, food.y);
    screenBuffer[pos] = '*';
}
void drawScore(unsigned int score) {
    char scoreBoard[MAX_WIDTH];
    memset(scoreBoard, ' ', sizeof(scoreBoard));
    snprintf(scoreBoard, sizeof(scoreBoard), "YOUR SCORE: %d", score);
    int pos = calcPosition(0, MAX_HEIGHT);
    memcpy(screenBuffer + pos, scoreBoard, strlen(scoreBoard));
}
void gameDraw(GameContext *context) {
    switch (context->state)
    {
        case STATE_GAME_RUNNING: {
            drawBackground();
            drawSnake(context->snake);
            drawFood(context->food);
            drawScore(context->score);
            break;
        }
        case STATE_GAME_PAUSE: {
            break;
        }
        case STATE_MENU: {
            drawBackground();
            drawMenu(context, screenBuffer);
            break; 
        }
        default:
            break;
    }
}

void gameRunningUpdate(GameContext *context) {
    Node **snake = &context->snake; 
    switch (context->direction)
    {
        case DIR_RIGHT: {
            int headPosX = (*snake)->x;
            headPosX = (headPosX + 1) % (MAX_WIDTH - 1);
            if (headPosX == 0) headPosX = 1;
            snakeAddHead(snake, headPosX, (*snake)->y);
            break;
        }
        case DIR_LEFT: {
            int headPosX = (*snake)->x;
            if (headPosX == 1) headPosX = MAX_WIDTH - 2;
            else headPosX--;
            snakeAddHead(snake, headPosX, (*snake)->y);
            break;
        }
        case DIR_UP: {
            int headPosY = (*snake)->y;
            if (headPosY == 1) headPosY = MAX_HEIGHT - 2;
            else headPosY--;
            snakeAddHead(snake, (*snake)->x, headPosY);
            break;
        }
        case DIR_DOWN: {
            int headPosY = (*snake)->y;
            headPosY = (headPosY + 1) % (MAX_HEIGHT - 1);
            if (headPosY == 0) headPosY = 1;
            snakeAddHead(snake, (*snake)->x, headPosY);
            break;
        }
        
        default:
        break;
    }
    bool getEat = ((*snake)->x == context->food.x && (*snake)->y == context->food.y);
    if (getEat) {
        generateFood(context);
        context->score += 100;
    } else {
        snakeRemoveTail(*snake);
    }

}

void gameUpdate(GameContext *context) {
    switch (context->state)
    {
        case STATE_GAME_RUNNING: {
            gameRunningUpdate(context);
            break;
        }
        case STATE_GAME_PAUSE: {
            break;
        }
        case STATE_MENU: {
            updateMenu(context);
            break;
        }
        default:
            break;
    }
    
}

void render(){
    printf("\033[H");
    printf("%s", screenBuffer);
    fflush(stdout);
}


void gameDeinit(GameContext *context) {
    Node *snake = context->snake;
    while (snake->next != NULL)
    {
        Node *nextNode = snake->next;
        free(snake);
        snake = nextNode;
    }
    free(context->snake);
    context->snake = NULL;
}

void newGameInit(GameContext *context) {
    memset(context, 0, sizeof(*context));
    snakeAddHead(&context->snake, 5, 5);
    snakeAddHead(&context->snake, 6, 5);
    snakeAddHead(&context->snake, 7, 5);
    
    context->state = STATE_GAME_RUNNING;
    context->isExit = false;
    context->direction = DIR_RIGHT;
    context->score = 0;

    context->food.x = 15;
    context->food.y = 10;
}

void gameRun(GameContext *context) {
    while (!context->isExit) {
        handleInput(context);
        gameUpdate(context);
        gameDraw(context);
        render();
        sleep(500);
    }
}