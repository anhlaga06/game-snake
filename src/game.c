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

void enterMenu1(GameContext *context) {
    context->state = STATE_MENU;
    context->menu.type = MENU_1;
    context->menu.active = 1;
}

void gameInit(GameContext *context) {
    printf("\033[2J");      // clear
    printf("\033[?25l");    // hide cursor
    
    memset(context, 0, sizeof(*context));
    context->isExit = false;

    enterMenu1(context);
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
                if (nextDirect == DIR_NONE) {
                    if (userInput == 27) {
                        context->state = STATE_MENU;
                        context->menu.type = MENU_2;
                        context->menu.active = 1;
                    }
                } else if (!isOppositeDirection(context->direction, nextDirect)) {
                    context->direction = nextDirect;
                }
                break;
            }
            case STATE_GAME_PAUSE: {
                break;
            }
            case STATE_GAME_OVER: {
                if (userInput == 13) {
                    gameDeinit(context);
                    enterMenu1(context);
                }
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


bool comparePoint(Point *c1, Point *c2) {
    return (c1->x == c2->x && c1->y == c2->y);
}

void generateFood(GameContext *context) {
    while (true)
    {
        Food *foodPtr = &context->food;
        foodPtr->c.x = (rand() % (MAX_WIDTH - 2)) + 1;
        foodPtr->c.y = (rand() % (MAX_HEIGHT - 2)) + 1;
        bool isFoodOverlapSnake = false;
        Node *node = context->snake;
        while (node)
        {
            if (comparePoint(&node->c, &foodPtr->c)) {
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
    int headPos = calcPosition(snake->c.x, snake->c.y);
    screenBuffer[headPos] = 'O';
    Node *snakeNode = snake->next;
    while (snakeNode)
    {
        int bodyPos = calcPosition(snakeNode->c.x, snakeNode->c.y);
        screenBuffer[bodyPos] = 'o';
        snakeNode = snakeNode->next;
    }
}
void drawFood(Food food) {
    int pos = calcPosition(food.c.x, food.c.y);
    screenBuffer[pos] = '*';
}
void drawScore(unsigned int score) {
    char scoreBoard[MAX_WIDTH];
    memset(scoreBoard, ' ', sizeof(scoreBoard));
    int pos = calcPosition(0, MAX_HEIGHT);
    memcpy(screenBuffer + pos, scoreBoard, MAX_WIDTH);
    snprintf(scoreBoard, sizeof(scoreBoard), "YOUR SCORE: %d", score);
    memcpy(screenBuffer + pos, scoreBoard, strlen(scoreBoard));
}
void drawGameOver(unsigned int score) {
    drawBackground();
    char buff[MAX_WIDTH];
    int pos = calcPosition(1, 1);
    snprintf(buff, sizeof(buff), "YOUR SCORE: %d", score);
    memcpy(screenBuffer + pos, buff, strlen(buff));
    pos = calcPosition(1, 3);
    snprintf(buff, sizeof(buff), "Press \"enter\" to new game!", score);
    memcpy(screenBuffer + pos, buff, strlen(buff));
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
        case STATE_GAME_OVER: {
            drawGameOver(context->score);
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

bool checkCollision(Node *snake) {
    Node *head = snake;
    CHECK(snake != NULL);
    while (snake->next != NULL)
    {
        snake = snake->next;
        if (comparePoint(&snake->c, &head->c)) {
            return true;
        }
    }
    return false;
}

void gameRunningUpdate(GameContext *context) {
    Node **snake = &context->snake; 
    switch (context->direction)
    {
        case DIR_RIGHT: {
            int headPosX = (*snake)->c.x;
            headPosX = (headPosX + 1) % (MAX_WIDTH - 1);
            if (headPosX == 0) headPosX = 1;
            snakeAddHead(snake, headPosX, (*snake)->c.y);
            break;
        }
        case DIR_LEFT: {
            int headPosX = (*snake)->c.x;
            if (headPosX == 1) headPosX = MAX_WIDTH - 2;
            else headPosX--;
            snakeAddHead(snake, headPosX, (*snake)->c.y);
            break;
        }
        case DIR_UP: {
            int headPosY = (*snake)->c.y;
            if (headPosY == 1) headPosY = MAX_HEIGHT - 2;
            else headPosY--;
            snakeAddHead(snake, (*snake)->c.x, headPosY);
            break;
        }
        case DIR_DOWN: {
            int headPosY = (*snake)->c.y;
            headPosY = (headPosY + 1) % (MAX_HEIGHT - 1);
            if (headPosY == 0) headPosY = 1;
            snakeAddHead(snake, (*snake)->c.x, headPosY);
            break;
        }
        
        default:
        break;
    }
    
    if (checkCollision(context->snake)) {
        context->state = STATE_GAME_OVER;
        return;
    }
    
    bool getEat = comparePoint(&(context->snake->c), &(context->food.c));
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
    while (snake != NULL)
    {
        Node *nextNode = snake->next;
        free(snake);
        snake = nextNode;
    }
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

    context->food.c.x = 15;
    context->food.c.y = 10;
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